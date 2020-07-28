#include "thread_pool.h"


ThreadPool::ThreadPool(std::size_t numThreads)
{
    if (!numThreads)
    {
        numThreads = std::thread::hardware_concurrency();
    }

    threads_.reserve(numThreads);
    for (std::size_t i{}; i < numThreads; ++i)
    {
        threads_.emplace_back(&ThreadPool::runTask, this);
    }
}

ThreadPool::~ThreadPool()
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
    }
}
