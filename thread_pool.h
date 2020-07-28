#pragma once

#include <atomic>
#include <future>
#include <type_traits>
#include <thread>
#include <vector>

#include "conc_queue.h"
#include "task_wrapper.h"


class ThreadPool
{
public:
    explicit ThreadPool(
        std::size_t numThreads = std::thread::hardware_concurrency());

    ~ThreadPool();

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    void addTask(TaskPtr&& f);

    std::size_t size() const;
private:
    void runTask();

    std::atomic_bool done_{};
    ConcurrentQueue<TaskPtr> queue_;
    std::vector<std::thread> threads_;
};
