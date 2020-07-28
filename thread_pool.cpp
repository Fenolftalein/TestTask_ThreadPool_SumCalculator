#include "thread_pool.h"


ThreadPool::ThreadPool()
{
    const std::size_t threadCount {
        std::max(std::thread::hardware_concurrency(), 4u)};

    threads_.reserve(threadCount);
    for (std::size_t i{}; i < threadCount; ++i)
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
