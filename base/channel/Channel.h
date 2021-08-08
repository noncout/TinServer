//
// Created by iocout on 6/20/20.
// 用于注册用户事件，每个channel只绑定一个fd
//

#ifndef TINSERVER_CHANNEL_H
#define TINSERVER_CHANNEL_H


#include <functional>
#include <time.h>
#include <chrono>
#include <vector>

#include "../noncopyable/noncopyable.h"
#include "../../webserver/Server.h"
#include "../log/logging.h"

class Channel : public enable_shared_from_this<Channel>, noncopyable {
public:
    typedef std::function<void(int fd)> EventCallback;
    typedef std::function<void(int fd, std::time_t)> ReadEventCallback;

    // 设置关联的fd
    Channel(int fd, Server * server);

    ~Channel() {
        Log::LOG_INFO << "Delete Channel, fd is:" << fd_;
    }

    void handleEvent(std::time_t time);

    void setConnectback(EventCallback cb)
    { connectCallback_ = std::move(cb); }

    // 连接事件的的回调需要手动再连接处回调，如果需要放入线程池也是可以的
    void connection()
    { if (connectCallback_) connectCallback_(fd()); }

    void setReadCallback(ReadEventCallback cb)
    { readCallback_ = std::move(cb); }

    void setWriteCallback(EventCallback cb)
    { writeCallback_ = std::move(cb); }

    void setCloseCallback(EventCallback cb)
    { closeCallback_ = std::move(cb); }

    void setErrorCallback(EventCallback cb)
    { errorCallback_ = std::move(cb); }

    int fd() const { return fd_; }

    std::vector<int> events() { return events_; }

    void set_events(int revt) { events_.push_back(revt); }
private:
    ReadEventCallback readCallback_;
    EventCallback connectCallback_;
    EventCallback writeCallback_;
    EventCallback errorCallback_;
    EventCallback closeCallback_;
    const int   fd_;
    bool        eventHandling_;
    std::vector<int>   events_;  // save the events type for every circle
    Server * server_;
    mutex   mutex_;
};


#endif //TINSERVER_CHANNEL_H
