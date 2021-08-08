#include "AsyncLog.h"

#include <assert.h>
#include <chrono>
#include <string.h>
#include <sys/time.h>

#include "FileUtil.h"
#include "LogFile.h"

using namespace std::chrono_literals;

using namespace Log;

Buffer::Buffer() {
    current_ = data_;
}

void Buffer::append(const char *logline, int length) {
    // 使用时保证了不会超出
    if(avail() > length) {
        memcpy(current_, logline, length);
        current_ += length;
    }
}

AsyncLog::AsyncLog(const std::string & fileName,
                   size_t  flushInterval,
                   uint64_t blockSize)
        : flush_interval_(flushInterval),
          latch_(1),
          current_buffer_(new Buffer),
          next_buffer_(new Buffer),
          buffers_(),
          file_(new LogFile(fileName, blockSize))
{
    current_buffer_->bZero();
    next_buffer_->bZero();
    buffers_.reserve(16);
}

void AsyncLog::start() {
    running_ = true;
    thread_ = std::thread(&AsyncLog::threadFunc, this);
    latch_.wait();
}

void AsyncLog::stop() {
    running_ = false;
    cond_.notify_all();
    thread_.join();
}

void AsyncLog::append(const char * logline, int len) {
    std::lock_guard<std::mutex>  lock(mutex_);
    if (current_buffer_->avail() > len) {
        current_buffer_->append(logline, len);
    } else {
        // 交换缓存
        buffers_.push_back(std::move(current_buffer_));
        // 已经写到日志中了
        // 如果next_buffer_已经交换到了current_buffer_中，
        // next_buffer_就会变为nullptr
        if (next_buffer_) {
            current_buffer_ = std::move(next_buffer_);
        } else {
            // 还没有写到日志中
            current_buffer_.reset(new Buffer);
        }
        current_buffer_->append(logline, len);
        // 通知消费者写
        cond_.notify_one();
    }
}

// 消费者线程,将消息写入到日志文件
void AsyncLog::threadFunc() {
    // TODO:配置直接输出的类
    BufferPtr newBuffer1(new Buffer);
    BufferPtr newBuffer2(new Buffer);
    newBuffer1->bZero();
    newBuffer2->bZero();
    BufferVector buffersToWrite;
    buffersToWrite.reserve(16);
    latch_.countDown();
    while (running_) {
        assert(newBuffer1 && newBuffer1->length() == 0);
        assert(newBuffer2 && newBuffer2->length() == 0);
        // 注意临界区
        {
            std::unique_lock<std::mutex>  lock(mutex_);
            // 设置成定时启动和通知启动
            cond_.wait_for(lock,
                           std::chrono::milliseconds(flush_interval_),
                           [=] { return !buffers_.empty(); });
            // 1.因为conditon而醒来，需要将当前的buffer写入
            // 2.因为时间到了而醒来，同样需要将当前的buffer写入
            buffers_.push_back(std::move(current_buffer_));
            // 归还一个buffer
            current_buffer_ = std::move(newBuffer1);
            // copy on write
            buffersToWrite.swap(buffers_);
            //  如果已经交换到第二个缓存，归还备用buffer
            if (!next_buffer_)
            {
                next_buffer_ = std::move(newBuffer2);
            }
        }
        assert(!buffersToWrite.empty());
        // 超长长度，舍掉多余的buffer，打印信息
        if (buffersToWrite.size() > 25) {
            char buf[256];
            struct timeval t;
            ::gettimeofday(&t, nullptr);
            std::string time = formaTime(t);
            snprintf(buf, sizeof(buf),
                     "Dropped log messages at %zd larger buffers\n",
                     buffersToWrite.size()-2);
            fputs(buf, stderr);
            file_->append(buf, static_cast<int>(strlen(buf)));
            // 删除多余的对列
            buffersToWrite.erase(buffersToWrite.begin()+2, buffersToWrite.end());
        }

        // 写到日志
        for (const auto& buffer : buffersToWrite)
        {
            file_->append(buffer->data(), buffer->length());
        }

        if (buffersToWrite.size() > 2)
        {
            // drop non-bzero-ed buffers, avoid trashing
            buffersToWrite.resize(2);
        }

        if (!newBuffer1)
        {
            assert(!buffersToWrite.empty());
            newBuffer1 = std::move(buffersToWrite.back());
            buffersToWrite.pop_back();
            newBuffer1->reset();
        }

        if (!newBuffer2)
        {
            assert(!buffersToWrite.empty());
            newBuffer2 = std::move(buffersToWrite.back());
            buffersToWrite.pop_back();
            newBuffer2->reset();
        }

        buffersToWrite.clear();
        file_->flush();
    }
    file_->flush();
}

std::string AsyncLog::formaTime(timeval tv) {
    char timestr[30];
    char buffer [30];
    auto dt = localtime(&tv.tv_sec);
    strftime(timestr, sizeof(timestr), "%Y-%M-%D %H:%m:%s", dt);
    return std::string(buffer);
}

void AsyncLog::flush() {
    file_->flush();
}

