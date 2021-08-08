//
// Created by iocout on 6/12/20.
//
#ifndef THREADPOOL_THREADPOOL_H
#define THREADPOOL_THREADPOOL_H
#include <mutex>
#include <condition_variable>
#include <vector>
#include <thread>
#include <functional>
#include <singleton/Singleton.h>

using namespace std;

class ThreadPool : public Singleton<ThreadPool> {

    typedef std::function<void ()> Task;
public:
    ThreadPool();

    void Init(int size);

    void Run();

    void AddTask(Task task);

    void Stop();

    // for debug
    void InitFunction(Task task);
private:
    void func();

    ThreadPool::Task take();
private:
    int exit_;
    int size_;
    mutex mutex_;
    std::vector<Task> task_list_;
    vector<thread> pools_;
    condition_variable cond_;
    Task init_task_;
};
#endif //TINSERVER_THREADPOOL_H
