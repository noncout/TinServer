#include <sys/socket.h>
#include "Echo.h"

EchoServer::EchoServer(Config &config)
    : server_(config)
{
    server_.setMessageCallback(std::bind(&EchoServer::onMessage,
            this, std::placeholders::_1, std::placeholders::_2));
    server_.setWriteCallback([](int fd){
        Log::LOG_INFO << "Echo Finished";
    });
    server_.setConnectionCallback([](int fd) {
        Log::LOG_INFO << "new fd:" << fd;
    });
}

void EchoServer::onMessage(int fd, time_t time) {
    char buffer[100];
    ::memset(buffer, 0x00, sizeof(buffer));
    std::string out;
    // 读取数据
    int ret = ::read(fd, buffer, 100);
    if ( ret > 0 ) {
        out.append(buffer);
        ::memset(buffer, 0x00, sizeof(buffer));
    } else {
        // shutdown(fd, SHUT_RDWR);
        server_.closeFd(fd);
        return;
    }
    ::write(fd, out.c_str(), out.size());
    Log::LOG_INFO << "Echo:" << out;
}

void EchoServer::runLoop() {
    server_.eventLoop();
}
