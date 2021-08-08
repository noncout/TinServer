//
// Created by iocout on 6/12/20.
//

#ifndef TINSERVER_SERVER_H
#define TINSERVER_SERVER_H

#include <iostream>
#include <vector>
#include <memory>
#include <atomic>
#include <map>
#include <ctime>
#include <functional>
#include <mutex>

#include "../config/Config.h"

class EpollPoller;
class Channel;
class ThreadPool;

class Server : std::enable_shared_from_this<Server>{
public:

    typedef std::map<int,shared_ptr<Channel>> ChannelList;

    typedef std::function<void(int fd)> EventCallback;

    typedef std::function<void(int fd, std::time_t)> ReadEventCallback;

    typedef std::map<int,EventCallback> TimerHandleList;
public:
    explicit Server(Config &config);

    ~Server();

    void eventLoop();

    void quit();

    void setConnectionCallback(const EventCallback & cb)
    {
        connectCallback_ = cb;
    }

    void setMessageCallback(const ReadEventCallback & cb)
    {
        readCallback_ = cb;
    }

    void setWriteCallback(const EventCallback & cb){
        writeCallback_ = cb;
    }

    void setCloseCallback(const EventCallback & cb)
    {
        closeCallback_ = cb;
    }

    void closeFd(int socketFd);

    // todo:定时器
    int addTimeOut(ReadEventCallback task, int ms);

    void stopTimeOut(int fd);

private:

    void init();

    void eventListen();

    void onListenEvent(int socketFd);

    void onEvent(int socketFd, int event);

protected:

    // new connection
    void onConnect(int socketFd);

    void threadPollLoops();
private:
    // status
    std::atomic<bool>   quit_;
    // config
    Config &            config_;
    //base
    int                 listenfd_;
    EventCallback                   connectCallback_;
    ReadEventCallback               readCallback_;
    EventCallback                   writeCallback_;
    EventCallback                   closeCallback_;
    std::shared_ptr<EpollPoller>    poller_;
    // threadpool
    std::shared_ptr<ThreadPool>      thread_polls_;
    // channel
    ChannelList         fullChannelList_;  // 全部连接列表，可以用时间轮来定时踢掉空闲连接
    ChannelList         actChannelList_; // 已激活列表
    TimerHandleList     timerHandleList_; // 定时器列表
};


#endif //TINSERVER_SERVER_H
