#include "config/Config.h"

#include <iostream>
#include <sstream>
#include <thread>
#include <ctime>
#include <functional>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <sys/socket.h>
#include <iomanip>

#include "base/log/logging.h"
#include "method/echo/Echo.h"
#include "webserver/Server.h"
#include "method/http/HttpHeader.h"
#include "method/http/HttpServer.h"
#include "base/thread_pool/ThreadPool.h"

using namespace std;

static string baidu = "";

// help func
string readFile() {
    ifstream file("../method/http/file/baidu.html");
    ostringstream stream;
    stream << file.rdbuf();
    baidu = stream.str();
    return baidu;
}

// echo
void TestEcho(Config config) {
    EchoServer echo(config);
    echo.runLoop();
}

// baidu
void TestBaidu(Config & config) {
    Server server(config);
    //![1] return http
    readFile();
    std::string httpHeader = HttpHeader::GetHeader();
    // header + body
    std::string http_baidu = httpHeader + baidu;
    server.setMessageCallback([&server, &http_baidu](int fd,time_t time) {
        char msg[512];
        ::memset(&msg, 0x00, sizeof(msg));
        int ret = read(fd, msg, sizeof(msg));
        LOG_INFO << "onMessage callback, ret: " << ret << " msg:" << msg;
        if (ret > 0)
        {
            LOG_INFO << "read size:" <<ret;
            LOG_INFO << "read:\n" << msg;
            ::memset(&msg, 0x00, sizeof(msg));
            int ret = ::write(fd,http_baidu.c_str(),http_baidu.size());
            if (ret < 0) {
                LOG_ERROR << "error to write, ret:" << ret << ", errno:" << strerror(errno);
            } else {
                LOG_INFO << "total write:" << ret << " bytes!";
            }
        } else if (ret == 0) {
            server.closeFd(fd);
            return;
        } else {
            LOG_INFO << "read file happend:" << strerror(errno);
        }
    });
    server.eventLoop();
    server.quit();
}


// http server
void TestHttp(Config & config){
    HttpServer httpServer(config);
    httpServer.runInLoop();
}

// threadpool
void TestThreadPool() {
    auto instance = ThreadPool::Instance();
    instance->Init(4);
    instance->Run();
    for (int i = 0; i < 15; ++i) {
        instance->AddTask([]() {
            LOG_INFO << "run thread:" << pthread_self();
            std::this_thread::sleep_for(2s);
        });
    }
}

// timer
void TestTimeOut(Config config)
{
    Server server(config);
    server.addTimeOut([](int fd, time_t time_t){
        uint64_t  times;
        read(fd, &times, sizeof(times));
        auto time = system_clock::now();
        auto now = system_clock::to_time_t(time);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&now), "%Y-%m-%d %X");
        cout << "now :" << ss.str() << endl;
    },1000);
    server.eventLoop();
    server.quit();
}

int main(int argc,char * argv[]) {
    Logging::Instance()->init();
    LOG_ERROR  << "start server";
    Config config;
    config.parse_arg(argc,argv);
    LOG_INFO << "Load config successed!";
    //![0] echo
//    TestEcho(config);
    //![1] read baidu
//    TestBaidu(config);
    //![2] open a html -> 404 not found
//    TestHttp(config);
    //![3] test threadpool
//    TestThreadPool();
    //![4] timer
    TestTimeOut(config);
    Logging::Instance()->stop();
    return 0;
}

