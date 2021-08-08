//
// Created by HF on 2021/1/24.
//

#ifndef TINSERVER_TIMERHANDLE_H
#define TINSERVER_TIMERHANDLE_H

// 定时器
class TimerHandle {
public:
    TimerHandle()= default;

    int Create(int ms);

private:
    int fd_;
};


#endif //TINSERVER_TIMERHANDLE_H
