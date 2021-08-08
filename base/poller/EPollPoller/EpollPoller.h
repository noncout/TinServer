//
// Created by iocout on 6/20/20.
//

#ifndef TINSERVER_EPOLLPOLLER_H
#define TINSERVER_EPOLLPOLLER_H

/// Use Epoll model
#include <chrono>
#include <vector>
#include <map>
#include <queue>
#include <sys/epoll.h>

class EpollPoller {
public:
    typedef std::vector<epoll_event> EventList;

    EpollPoller();

    ~EpollPoller();

    int addFd(int fd, bool oneShot, int triggleMode);

    int delFd(int fd);

    int wait();

    std::vector<epoll_event> getActivateFd() const;

private:

    int setNonblock(int fd);

private:
    int epollfd_;
    EventList events_;
    EventList events_queue_;
    const static int kInitEventSize = 16;
};

#endif //TINSERVER_EPOLLPOLLER_H
