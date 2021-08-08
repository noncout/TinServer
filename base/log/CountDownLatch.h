#ifndef LOGGING_COUNTDOWNLATCH_H
#define LOGGING_COUNTDOWNLATCH_H

#include <atomic>
#include <mutex>
#include <condition_variable>

#include "../noncopyable/noncopyable.h"

class CountDownLatch : public noncopyable {
public:

    explicit CountDownLatch(int count);

    void wait();

    void countDown();

    int getCount();

private:
    std::atomic<int>    count_;
    std::mutex          mutex_;
    std::condition_variable cond_;
};


#endif //LOGGING_COUNTDOWNLATCH_H
