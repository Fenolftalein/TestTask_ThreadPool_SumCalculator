//
// Created by vladislav.saenko on 28.07.2020.
//

#pragma once

#include "thread_pool.h"

class Calculator;

struct AverageWorker : TaskWrapper
{
public:
    AverageWorker(Calculator& calculator);
    void operator()();
private:
    std::reference_wrapper<Calculator> calculator_;
};

class Calculator
{
public:
    using NumType = std::int64_t;
    using Container = std::vector<NumType>;

    Calculator(const Container& c, ThreadPool& tp);
    void run();
    std::future<double> result();
private:
    friend class AverageWorker;

    void calculate();

    std::reference_wrapper<const Container> data_;
    std::reference_wrapper<ThreadPool> tp_;

    std::mutex mutex_;
    std::atomic<NumType> sum_{};
    std::atomic<std::size_t> idx_{};
    std::promise<double> result_;
};
