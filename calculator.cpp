//
// Created by vladislav.saenko on 28.07.2020.
//

#include "calculator.h"

AverageWorker::AverageWorker(Calculator& calculator)
    : calculator_{calculator}
{}

void AverageWorker::operator()()
{
    calculator_.get().calculate();
}


Calculator::Calculator(
    const Calculator::Container& c,
    ThreadPool& tp)
    : data_{c}
    , tp_{tp}
{}


void Calculator::run()
{
    const auto size{ data_.get().size() };
    for (std::size_t i{}; i < size; ++i)
    {
        tp_.get().addTaskNoReturn(AverageWorker(*this));
    }
}

void Calculator::calculate()
{
    std::lock_guard lg{mutex_};
    const auto& v = data_.get();
    const auto size{v.size()};

    if (idx_ == size) return;
    else
    {
        sum_ += v.at(idx_++);
        // do not return for the last number to calc average
        if (idx_ != size) return;
    }
    result_.set_value(sum_ * 1.0 / size);
}


std::future<double> Calculator::result()
{
    return result_.get_future();
}
