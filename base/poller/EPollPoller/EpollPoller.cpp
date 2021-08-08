//
// Created by iocout on 6/20/20.
//

#include "EpollPoller.h"
#include "../../log/logging.h"
#include <sys/epoll.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <algorithm>
#include <string.h>

EpollPoller::EpollPoller() {
    epollfd_ = epoll_create(5);
    events_ = EventList(kInitEventSize);
}

EpollPoller::~EpollPoller() {
    close(epollfd_);
}

int EpollPoller::delFd(int fd) {
    assert(fd >= 0);
    int ret = epoll_ctl(epollfd_, EPOLL_CTL_DEL, fd, 0);
    if (ret < 0) {
        LOG_INFO << "delFd error";
    }
    ret = ::close(fd);
    if (ret < 0){
        LOG_INFO << "close fd :"<< fd << ", error, reason:" << strerror(errno);
    }
    return ret;
}

// EPOLLRDHUP : half close   EPOLLET: edge triggled
int EpollPoller::addFd(int fd, bool oneShot, int triggleMode) {
    assert(fd >= 0);
    epoll_event event;

    if (triggleMode == 1) {
        event.events = EPOLLIN | EPOLLRDHUP | EPOLLET;
    } else {
        event.events = EPOLLIN;
    }

    if (oneShot) {
        // 单个线程可操作
        event.events |= EPOLLONESHOT;
    }
    event.data.fd = fd;
    int ret = epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd, &event);
    setNonblock(fd);
    return ret;
}

int EpollPoller::setNonblock(int fd) {
    assert(fd >= 0);
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_ASYNC;
    fcntl(fd, F_SETFD, new_option);
    return old_option;
}

int EpollPoller::wait() {
    std::vector<epoll_event>().swap(events_queue_);
    int ret = epoll_wait(epollfd_, &*events_.begin(),
                         static_cast<int>( events_.size()), -1);
    if ((ret < 0) && (errno != EINTR)) {
        LOG_INFO << "Error Epoll_wait in EpollPoller,reason:" << strerror(errno) ;
        abort();
    } else if (ret == 0) {
        LOG_INFO << "Epoll_wait nothing happen";
//        return 0;
    } else {
        for (int i = 0; i < ret; ++i) {
            events_queue_.push_back(events_[i]);
        }
        if (events_.size() == ret) {
            events_.resize(events_.size() * 2);
        }
        return 1;
    }
    return 0;
}

std::vector<epoll_event> EpollPoller::getActivateFd() const {
    return events_queue_;
}
