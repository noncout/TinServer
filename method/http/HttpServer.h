//
// Created by HF on 2020/11/25.
//

#ifndef TINSERVER_HTTPSERVER_H
#define TINSERVER_HTTPSERVER_H


#include "../../webserver/Server.h"

class HttpServer {
public:
    explicit HttpServer(Config & config);

    ~HttpServer();

    void runInLoop();

    void onMessage(int fd, time_t time);

private:
    // 读html文件
    std::string readFile(const std::string & file);

    // 解析请求的文件
    std::string parseHeader(std::string context);

private:
    Server server_;
};

#endif //TINSERVER_HTTPSERVER_H
