//
// Created by iocout on 6/20/20.
//

#include <sys/epoll.h>
#include "Channel.h"


Channel::Channel(int fd, Server * server)
        : fd_(fd),
          eventHandling_(false),
          events_(0),
          server_(server)
{
    //
}

void Channel::handleEvent(std::time_t time) {
    std::lock_guard<mutex>  lock(mutex_);
    eventHandling_ = true;
    // 处理时间列表中的所有事件
    for(const auto & event : events_) {
        //LOG_DEBUG << "channel get event:" << event;
        //LOG_DEBUG << "EPOLLERR:" << EPOLLERR << " EPOLLIN:" << EPOLLIN  << " EPOLLOUT:" << EPOLLOUT << " EPOLLIN | EPOLLRDHUP:" << (EPOLLIN | EPOLLRDHUP);
        if (event & (EPOLLERR)) {
            if (errorCallback_) {
                errorCallback_(fd_);
            }
            if (server_!= nullptr) {
                server_->closeFd(fd_);
            }
        } else if (event & EPOLLRDHUP) {
            // 一般不会触发这个
            if (closeCallback_) {
                closeCallback_(fd_);
            }
            if (server_ != nullptr) {
                server_->closeFd(fd_);
            }
        } else if (event & (EPOLLIN)) {
            if (readCallback_) {
                readCallback_(fd_, time);
            }
        } else if (event & (EPOLLOUT)) {
            if (writeCallback_) {
                writeCallback_(fd_);
            }
        } else {
            LOG_ERROR << "**********************************************";
            LOG_ERROR <<"Channel get Error Event fd:" << fd_ << " Event type:" << event;
            LOG_ERROR <<"**********************************************\n";
        }
    }
    eventHandling_ = false;
}
