#pragma once

#include <atomic>
#include <future>
#include <type_traits>
#include <thread>
#include <vector>

#include "conc_queue.h"
#include "func_wrapper.h"


class ThreadPool
{
public:
    ThreadPool();
    ~ThreadPool();

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    template <typename Func>
    std::future<std::result_of_t<Func()>> addTask(Func f);

private:
    void runTask();

    std::atomic_bool done_{};
    ConcurrentQueue<TaskWrapper> queue_;
    std::vector<std::thread> threads_;
};

#include "thread_pool.inl"
