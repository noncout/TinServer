#ifndef LOGGING_ASYNCLOG_H
#define LOGGING_ASYNCLOG_H

#include <string.h>

#include <mutex>
#include <vector>
#include <iostream>
#include <thread>
#include <condition_variable>

#include "CountDownLatch.h"
#include "LogFile.h"

class FileUitl;
class LogFile;

namespace  Log {
// 定长buffer：4M
    const int kLargeBuffer = 4 * 1000 * 1000;

// 定长日志缓冲区
    class Buffer {
    public:

        Buffer();

        /**
         * 剩余可用
         */
        int avail() { return static_cast<int>(end() - current_); };

        char *data() { return data_; }

        void bZero() { memset(data_, 0x00, sizeof(*data_)); }

        void append(const char *logline, int length);

        int length() { return static_cast<int>(current_ - data_); }

        void reset() {
            bZero();
            current_ = data_;
        }

    private:
        char *end() { return data_ + sizeof(data_); }

    private:
        char data_[kLargeBuffer];
        char *current_;
    };

    class AsyncLog {

        typedef std::unique_ptr<Buffer> BufferPtr;

        typedef std::vector<BufferPtr> BufferVector;

    public:
        AsyncLog(const std::string & fileName,
                size_t  flushInterval = 1000,
                uint64_t blockSize = MAXSIZE);

        void append(const char *logline, int len);

        void start();

        void flush();

        void stop();

    private:

        // 这个线程是消费者，主线程是生产者
        void threadFunc();

        std::string formaTime(timeval tv);

    private:
        // 整体采用copy on write 的格式进行读写
        int flush_interval_; // 刷新间隔时间
        std::mutex mutex_;
        std::condition_variable cond_; // 条件变量，通知消息进行写
        std::thread thread_; // 异步写日志线程
        std::atomic<bool> running_;
        BufferPtr current_buffer_;
        BufferPtr next_buffer_;
        BufferVector buffers_; //
        CountDownLatch latch_;   // 用于控制启动
        std::unique_ptr<LogFile> file_;  // 用于真正的文件写
    };

} // Log
#endif //LOGGING_ASYNCLOG_H
