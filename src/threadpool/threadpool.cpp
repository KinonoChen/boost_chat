#include "threadpool.h"

ThreadPool::ThreadPool(size_t size) : stop(false) {
    for (size_t i = 0; i < size; ++i) {
        workers.emplace_back([this] { workerThread(); });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(jobs_mutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread &worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

void ThreadPool::execute(std::function<void()> job) {
    {
        std::unique_lock<std::mutex> lock(jobs_mutex);
        jobs.push(std::move(job));
    }
    condition.notify_one();
}

void ThreadPool::workerThread() {
    while (true) {
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lock(jobs_mutex);
            condition.wait(lock, [this] { return !jobs.empty() || stop; });
            if (stop && jobs.empty()) {
                return;
            }
            job = std::move(jobs.front());
            jobs.pop();
        }
        job();
    }
}