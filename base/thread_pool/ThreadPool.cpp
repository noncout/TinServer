//
// Created by iocout on 6/12/20.
//

#include "ThreadPool.h"
#include <assert.h>
#include "log/logging.h"

using namespace Log;

ThreadPool::ThreadPool() {
    size_ = 4;
    exit_ = false;
}

void ThreadPool::Init(int size) {
    exit_ = false;
    size_ = size;
}

void ThreadPool::Run() {
    if (exit_) {
        LOG_FATAL << "areadly exit!\n";
        exit(0);
    }
    assert(!exit_);
    for (int i = 0; i < size_; ++i) {
        auto th = thread(&ThreadPool::func, this);
        pools_.emplace_back(std::move(th));
    }
}

void ThreadPool::AddTask(Task task) {
    std::lock_guard<mutex> lock(mutex_);
    task_list_.push_back(std::move(task));
    cond_.notify_one();
}

void ThreadPool::Stop() {
    {
        // 这里必须要加大括号, 不然条件变量通知后锁无法释放，其他线程无法获得锁
        lock_guard<mutex> lock(mutex_);
        exit_ = true;
        cond_.notify_all();
    }
    for (auto & item : pools_) {
        item.join();
    }
}

void ThreadPool::func() {
    // debug
    if (init_task_) {
        init_task_();
    }
    while(!exit_) {
        Task task(take());
        if (task) {
            task();
        }
    }
}

ThreadPool::Task ThreadPool::take() {
    unique_lock<mutex> lock(mutex_);
    cond_.wait(lock, [&]() {
        int ret = !exit_ && task_list_.empty();
        return !ret;
    });
    Task task;
    if(!task_list_.empty()) {
        task = task_list_[0];
        task_list_.erase(task_list_.begin());
    }
    return task;
}

void ThreadPool::InitFunction(ThreadPool::Task task) {
    init_task_ = task;
}
