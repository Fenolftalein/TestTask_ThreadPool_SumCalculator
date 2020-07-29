//
// Created by vladislav.saenko on 28.07.2020.
//

#include "calculator.h"
#include <numeric>


Calculator::Calculator(const Calculator::Container& c)
    : data_{c}
    , size_{c.size()}
{}

double Calculator::get()
{
    return result_.get_future().get();
}

void Calculator::run(ThreadPool& tp, Calculator::TYPE type)
{
    (type == TYPE::PAIRS) ? runByPairs(tp) : runByRanges(tp);
}

//------------------------------------------------------------------

void Calculator::runByPairs(ThreadPool& tp)
{
    for (std::size_t i{}; i < size_; ++i)
    {
        auto task = [this](){ calculate(); };
        tp.addTask(task);
    }
}

void Calculator::runByRanges(ThreadPool& tp)
{
    const auto blocksCount {tp.size() - 1};
    const std::size_t blockSize = size_ / blocksCount;

    auto blockStart = data_.get().begin();
    for(std::size_t i{}; i < blocksCount; ++i)
    {
        auto blockEnd =
            (i != blocksCount - 1)
            ? blockStart + blockSize
            : data_.get().end();

        auto task = [this, blockStart, blockEnd, blocksCount]
        { return calculate(blockStart, blockEnd, blocksCount); };

        tp.addTask(task);

        blockStart = blockEnd;
    }
}

// one-by-one
void Calculator::calculate()
{
    if (size_ == counter_) return;

    {
        std::lock_guard lg{mutexForPairs_};
        sum_ += data_.get()[counter_];
        if (++counter_ != size_) return;
    }

    result_.set_value(sum_ * 1.0 / size_);
}

// by-ranges
void Calculator::calculate(
    Calculator::IterType first,
    Calculator::IterType last,
    const std::size_t count)
{
    if (count == counter_) return;

    sum_ += std::accumulate(first, last, NumType{});
    if (++counter_ != count) return;

    result_.set_value(sum_ * 1.0 / size_);
}
