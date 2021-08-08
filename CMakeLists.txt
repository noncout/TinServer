cmake_minimum_required(VERSION 3.10)
project(TinServer)

set(CMAKE_CXX_STANDARD 14)

SET(CMAKE_BUILD_TYPE "Debug")

SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${PROJECT_SOURCE_DIR}/bin)

include_directories(base)

aux_source_directory(base/log LOG_SRC)

add_executable(TinServer
        ${LOG_SRC}
        main.cpp
        config/Config.cpp
        config/Config.h
        webserver/Server.cpp
        webserver/Server.h
        base/thread_pool/ThreadPool.cpp
        base/thread_pool/ThreadPool.h
        base/noncopyable/noncopyable.h
        test/test_config.cpp
        test/test_thread_pool.cpp
        test/test_Singleton.cpp
        base/poller/EPollPoller/EpollPoller.cpp
        base/poller/EPollPoller/EpollPoller.h
        base/channel/Channel.cpp
        base/channel/Channel.h
        base/singleton/Singleton.h
        base/log/logging.h
        base/log/logging.cpp
        base/log/LogFile.h
        base/log/LogFile.cpp
        base/log/AsyncLog.cpp
        base/log/AsyncLog.h
        base/log/CountDownLatch.h
        base/log/CountDownLatch.cpp
        base/log/FileUtil.h
        base/log/FileUtil.cpp
        base/log/FormatTime.h
        base/log/FormatTime.cpp
        base/log/LogStream.h
        base/log/LogStream.cpp
        base/timer/TimerHandle.cpp
        base/timer/TimerHandle.h
        method/http/HttpServer.cpp
        method/http/HttpServer.h
        method/echo/Echo.cpp
        method/echo/Echo.h
        method/PublicHeader.h
        method/http/HttpHeader.cpp
        method/http/HttpHeader.h
        )

target_link_libraries(${PROJECT_NAME}  -lpthread)

