
#pragma once

#include <string>
#include <random>
#include <limits>
#include <iostream>
#include <chrono>

#include <atomic>
#include "conc_queue.h"

class TimeLogger
{
public:
    using time_type = std::chrono::steady_clock;

    TimeLogger(const std::string msg)
    {
        std::cout << msg << std::endl;
        start_ = time_type::now();
    }

    ~TimeLogger()
    {
        const auto dur {time_type::now() - start_};
        auto mks = std::chrono::duration_cast<std::chrono::microseconds>(dur);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur);
        std::cout << "Duration: ";
        if (ms.count() < 1)
        {
            std::cout << mks.count() << " mks\n";
        }
        else
        {
            std::cout << ms.count() << " ms\n";
        }
    }
private:
    std::chrono::time_point<time_type> start_;
};

//==================================================================

class RandomGenerator
{
public:
    RandomGenerator(int low=1, int high=1000)
    : dre_{std::random_device{}()}
    , distr_{low, high}
    {}

    int operator()()
    {
        return distr_(dre_);
    }
private:
    std::default_random_engine dre_;
    std::uniform_int_distribution<int> distr_;
};

//==================================================================
template <typename T = std::int64_t>
class ProducerConsumerTester
{
public:
    using QueueType = ConcurrentQueue<T>;

    void producer(T count, QueueType& q)
    {
        for (T i{}; i < count; ++i)
        {
            q.push(i);
            // for demonstration of parallel work
            std::cout << "thread "
                << std::this_thread::get_id() << ": pushed " << i << "\n";
        }
        done_ = true;
    }

    void consumer(QueueType& q)
    {
        while (true)
        {
            T item{};
            q.wait_pop(item);
            res_ += item;
            // for demonstration of parallel work
            std::cout << "thread "
                << std::this_thread::get_id() << ": got " << item << "\n";

            if (done_ && q.empty())
                break;
        }
    }

    // for test
    T res_{};
    std::atomic_bool done_{};
};
