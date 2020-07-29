//
// Created by vladislav.saenko on 28.07.2020.
//

#pragma once

#include "thread_pool.h"


class Calculator
{
public:
    using NumType = std::int64_t;
    using Container = std::vector<NumType>;
    using IterType = typename Container::const_iterator;

    explicit Calculator(const Container& c);

    enum class TYPE
    {
        PAIRS,
        RANGES
    };

    void run(ThreadPool& tp, TYPE type);

    double get();

private:
    void runByPairs(ThreadPool& tp);
    void calculate();

    void runByRanges(ThreadPool& tp);

    void calculate(
        IterType first,
        IterType last,
        const std::size_t count);

    std::reference_wrapper<const Container> data_;
    const std::size_t size_;

    std::mutex mutexForPairs_;
    std::atomic<std::size_t> counter_{};
    std::atomic<NumType> sum_{};
    std::promise<double> result_;
};
