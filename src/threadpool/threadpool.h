#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <memory>

class ThreadPool {
public:
    explicit ThreadPool(size_t size);
    ~ThreadPool();

    void execute(std::function<void()> job);

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> jobs;
    std::mutex jobs_mutex;
    std::condition_variable condition;
    bool stop;

    void workerThread();
};



#endif // THREADPOOL_H