//
// Created by vladislav.saenko on 29.07.2020.
//

#pragma once

#include "thread_pool.h"

class CalculatorBase
{
public:
    using NumType = std::int64_t;
    using Container = std::vector<NumType>;

    explicit CalculatorBase(const Container& c)
        : data_{c}
        , size_{c.size()}
    {}

    std::future<double> result() { return result_.get_future(); }

    virtual void run(ThreadPool&) {}

protected:
    std::reference_wrapper<const Container> data_;
    const std::size_t size_;

    std::atomic<NumType> sum_{};
    std::promise<double> result_;
};
