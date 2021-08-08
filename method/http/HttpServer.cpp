//
// Created by HF on 2020/11/25.
//

#include "HttpServer.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>

#include "HttpHeader.h"
#include "../../base/log/logging.h"
#include <algorithm>

HttpServer::HttpServer(Config &config)
        :server_(config)
{
    server_.setMessageCallback(std::bind(&HttpServer::onMessage,
            this,std::placeholders::_1, std::placeholders::_2));
}

HttpServer::~HttpServer() {
    server_.quit();
}

void HttpServer::runInLoop() {
    server_.eventLoop();
}

void HttpServer::onMessage(int fd, time_t time) {
    char buffer[1024*1024];
    memset(buffer, 0x00, sizeof(buffer));
    int ret = ::read(fd, buffer, sizeof(buffer));
    LOG_INFO << "ret:" << ret << " ,buffer:" << buffer;
    if (ret > 0) {
        std::string context(buffer);
        auto road = parseHeader(context);
        auto head = HttpHeader::GetHeader();
        auto body = readFile(road);
        auto send = head + body;
        ::write(fd, send.c_str(), send.size());
    } else if (ret == 0) {
        server_.closeFd(fd);
        LOG_DEBUG << "Http Close fd:" << fd;
    } else {
        LOG_FATAL << "Http error fd:" << fd << ",reason:" << strerror(errno);
    }
}

std::string HttpServer::readFile(const string &file) {
    std::ifstream fin(file);
    if (!fin.is_open()) {
        fin = ifstream("../404.html");
    }
    std::stringstream stream;
    stream << fin.rdbuf();
    return stream.str();
}

std::string HttpServer::parseHeader(std::string context) {
    Log::LOG_INFO << "Get Context:\n" << context;
    std::stringstream stream;
    stream << "../source/file";
    std::string splite("\n");
    auto index = std::search(context.begin(), context.end(), splite.begin(),splite.end());
    auto head = string(context.begin(), index);
    splite = "/";
    index = std::search(head.begin(), head.end(), splite.begin(),splite.end());
    auto bottom = string(index, head.end());
    splite = "HTTP/1.1";
    index = std::search(bottom.begin(), bottom.end(), splite.begin(), splite.end());
    auto uri = string(bottom.begin(), index);
    // trim
    uri.erase(0, uri.find_first_not_of(" "));
    uri.erase(uri.find_last_not_of(" ") + 1);
    // head page
    if (uri == "/") {
        uri = "/404.html";
    }
    stream << uri;
    Log::LOG_INFO << "stream:" << stream.str();
    return stream.str();
}
