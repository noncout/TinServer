//
// Created by iocout on 6/12/20.
//

#include "Server.h"
#include <sys/socket.h>
#include <assert.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
#include <cstring>
#include <memory>
#include <algorithm>
#include <timer/TimerHandle.h>

#include "log/logging.h"
#include "channel/Channel.h"
#include "thread_pool/ThreadPool.h"
#include "poller/EPollPoller/EpollPoller.h"

using namespace Log;

Server::Server(Config &config)
        : quit_(false),
          config_(config),
          poller_(std::make_shared<EpollPoller>()) {
    init();
}

Server::~Server() {
}

void Server::init() {
    //![1] 启动线程池
    thread_polls_ = std::shared_ptr<ThreadPool>(ThreadPool::Instance());
    thread_polls_->Init(4);
    thread_polls_->InitFunction([]() {
        LOG_INFO << "Create thread:" << syscall(SYS_gettid);
    });
}

void Server::quit() {
    quit_ = true;
    thread_polls_->Stop();
}

void Server::eventLoop() {
    assert(!quit_);
    thread_polls_->Run();
    eventListen();
    while (!quit_) {
        if (poller_->wait() == 1) {
            auto event_list = poller_->getActivateFd();
            for (int i = 0; i < event_list.size(); ++i) {
                int socket_fd = event_list[i].data.fd;
                if (socket_fd == listenfd_)
                    onListenEvent(socket_fd);
                else
                    onEvent(socket_fd, event_list[i].events);
            }
            // 将激活的任务放入线程池中
            threadPollLoops();
        } else {
            LOG_INFO << "poll wait return != 1";
            exit(-1);
        }
    }
    LOG_INFO << "success stop event loop";
}

void Server::eventListen() {
    //![0]
    listenfd_ = ::socket(PF_INET, SOCK_STREAM, 0);
    assert(listenfd_ >= 0);
    //![1] there, set SO_LINGER option
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(config_.PORT);

    int ret = 0;
    int flag = 1;
    // 端口释放后立即就可以被再次使用
    ret = setsockopt(listenfd_,
                     SOL_SOCKET,
                     SO_REUSEADDR,
                     static_cast<void * >(&flag),
                     static_cast<socklen_t>(sizeof(address)));
    assert(ret >= 0);
    ret = ::bind(listenfd_, (struct sockaddr *) &address,
                 static_cast<socklen_t>(sizeof(address)));
    assert(ret >= 0);
    ret = ::listen(listenfd_, 5);
    assert(ret >= 0);
    poller_->addFd(listenfd_, false, 0);
    Log::LOG_INFO << "listen fd is:" << listenfd_;
}

void Server::threadPollLoops() {
    for (auto iter = actChannelList_.begin(); iter != actChannelList_.end();) {
       thread_polls_->AddTask([=]() -> void {
           iter->second->handleEvent(time(NULL));
       });
        // 测试单线程性能
        // iter->second->handleEvent(time(NULL));
        iter = actChannelList_.erase(iter);
    }
}

void Server::onConnect(int socketFd) {
    std::shared_ptr<Channel> channel = std::shared_ptr<Channel>(new Channel(socketFd, this));
    if (connectCallback_) {
        channel->setConnectback(connectCallback_);
    }
    if (readCallback_) {
        channel->setReadCallback(readCallback_);
    }
    if (writeCallback_) {
        channel->setWriteCallback(writeCallback_);
    }
    if (closeCallback_) {
        channel->setCloseCallback(closeCallback_);
    }
    // Fixme:如果立马执行的是一个很耗时的操作，应该放入线程池中
    channel->connection();
    fullChannelList_[socketFd] = channel;
}

void Server::onListenEvent(int socketFd) {
    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    int conn_fd = ::accept(socketFd, reinterpret_cast<struct
            sockaddr *>(&client_addr), &client_addr_size);
    if (conn_fd < 0) {
        LOG_ERROR << "accept error,reason" << strerror(errno);
        exit(1);
    }
    poller_->addFd(conn_fd, false, 0);
    onConnect(conn_fd);
}

void Server::onEvent(int socketFd, int event) {
    auto index = fullChannelList_.find(socketFd);
    if (index != fullChannelList_.end()) {
        auto channel = index->second;
        channel->set_events(event);
        actChannelList_[socketFd] = channel;
    } else {
        printf("error to get fd :%d from fullChannelList!\n", socketFd);
        exit(-1);
    }
}

// 在channle中调用del fd
void Server::closeFd(int socketFd) {
    auto full_iter = fullChannelList_.find(socketFd);
    if ( full_iter != fullChannelList_.end()) {
        fullChannelList_.erase(socketFd);
        poller_->delFd(socketFd);
    }
}

int Server::addTimeOut(Server::ReadEventCallback task, int ms) {
    TimerHandle timer;
    if (ms <= 0) {
        return -1;
    }
    int fd = timer.Create(ms);
    LOG_INFO << "create timer fd:" << fd;
    poller_->addFd(fd, false, 0);
    std::shared_ptr<Channel> channel =
            std::shared_ptr<Channel>(new Channel(fd, this));
    channel->setReadCallback(std::bind(task, fd, std::placeholders::_1));
    fullChannelList_[fd] = channel;
    return fd;
}

void Server::stopTimeOut(int fd) {
    closeFd(fd);
}



