//
// Created by vladislav.saenko on 29.07.2020.
//

#include "calculator_range.h"
#include <numeric>

// Class for one task
struct CalculatorRange::Worker : Task
{
    Worker(
        CalculatorRange& calculator,
        IterType first,
        IterType last)
        : calculator_{calculator}
        , first_{first}
        , last_{last}
    {}

    void operator()() override
    {
        calculator_.get().calculate(first_, last_);
    }
private:
    std::reference_wrapper<CalculatorRange> calculator_;
    IterType first_;
    IterType last_;
};

//=====================================================


CalculatorRange::CalculatorRange(const CalculatorBase::Container& c)
    : CalculatorBase(c)
{}

void CalculatorRange::run(ThreadPool &tp)
{
    const auto blockSumsCount {tp.size() - 1};
    const std::size_t blockSize = size_ / blockSumsCount;

    auto blockStart = data_.get().begin();
    for(std::size_t i{}; i < blockSumsCount; ++i)
    {
        auto blockEnd =
            (i != blockSumsCount - 1)
            ? blockStart + blockSize
            : data_.get().end();

        tp.addTask(std::make_unique<Worker>(
            *this,
            blockStart,
            blockEnd));

        blockStart = blockEnd;
    }
}


void CalculatorRange::calculate(
    CalculatorRange::IterType first,
    CalculatorRange::IterType last)
{
    if (finished_)
        return;

    sum_ += std::accumulate(first, last, NumType{});

    if (last == data_.get().end())
    {
        result_.set_value(sum_ * 1.0 / size_);
        finished_ = true;
    }
}
