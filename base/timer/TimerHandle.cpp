//
// Created by HF on 2021/1/24.
//

#include "TimerHandle.h"
#include <sys/timerfd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "log/logging.h"

int TimerHandle::Create(int ms) {
    struct itimerspec ts;
    //![0] create timer
    fd_ = timerfd_create(CLOCK_REALTIME, 0);
    //![1] set timer
    ts.it_interval.tv_sec = ms / 1000;
    ts.it_interval.tv_nsec = (ms % 1000) * 1000000;
    ts.it_value.tv_sec = ms / 1000;
    ts.it_value.tv_nsec = (ms % 1000) * 1000000;
    int ret = timerfd_settime(fd_, 0, &ts, NULL);
    if (ret < 0) {
        LOG_INFO  <<  "timerfd_settime() failed, reason:%s" << strerror(errno);
        close(fd_);
        return -1;
    }
    return fd_;
}