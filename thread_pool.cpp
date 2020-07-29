#include "thread_pool.h"
#include <iostream>


ThreadPool::ThreadPool(std::size_t numThreads)
{
    if (!numThreads)
    {
        numThreads = 4;
    }

    threads_.reserve(numThreads);
    try
    {
        for (std::size_t i{}; i < numThreads; ++i)
        {
            threads_.emplace_back(&ThreadPool::runTask, this);
        }
    }
    catch(...)
    {
        std::cerr << "Caught an exception in ThreadPool c-tor!\n";
        destroy();
        throw;
    }
}

ThreadPool::~ThreadPool()
{
    destroy();
}

void ThreadPool::destroy()
{
    done_ = true;
    for (auto& t : threads_)
    {
        if (t.joinable())
        {
            t.join();
        }
    }
}

void ThreadPool::runTask()
{
    while (!done_)
    {
        if (decltype(queue_)::value_type task; queue_.try_pop(task))
        {
            task();
        }
        else
        {
            std::this_thread::yield();
        }
    }
}

std::size_t ThreadPool::size() const
{
    return threads_.size();
}
