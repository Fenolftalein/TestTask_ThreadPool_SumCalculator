#pragma once

#include <atomic>
#include <future>
#include <type_traits>
#include <thread>
#include <vector>

#include "conc_queue.h"


class ThreadPool
{
public:
    explicit ThreadPool(
        std::size_t numThreads = std::thread::hardware_concurrency());

    ~ThreadPool();

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    using FuncType = std::function<void()>;

    template <typename Func>
    void addTask(Func f)
    {
        queue_.push(FuncType(f));
    }

    std::size_t size() const;
private:
    void destroy();
    void runTask();

    std::atomic_bool done_{};
    ConcurrentQueue<FuncType> queue_;
    std::vector<std::thread> threads_;
};
