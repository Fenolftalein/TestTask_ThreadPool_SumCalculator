//
// Created by vladislav.saenko on 27.07.2020.
//

#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class ConcurrentQueue
{
public:
    using value_type = T;
    ConcurrentQueue() = default;
    void push(T value);
    void wait_pop(T& value);
    bool try_pop(T& value);
    bool empty() const;
private:
    mutable std::mutex mutex_;
    std::queue<T> queue_;
    std::condition_variable cv_;
};


#include "conc_queue.inl"
