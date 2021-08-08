#ifndef TINSERVER_ECHO_H
#define TINSERVER_ECHO_H


#include "../PublicHeader.h"

class EchoServer {
public:

    explicit EchoServer(Config &config);

    ~EchoServer() {
        server_.quit();
    }

    void runLoop();
private:

    void onMessage(int fd, time_t time);

private:
    Server server_;
};


#endif //TINSERVER_ECHO_H
