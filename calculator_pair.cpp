//
// Created by vladislav.saenko on 28.07.2020.
//

#include "calculator_pair.h"


// Class for one task
struct CalculatorPair::Worker : Task
{
    Worker(CalculatorPair& calculator)
        : calculator_{calculator}
    {}

    void operator()() override
    {
        calculator_.get().calculate();
    }
private:
    std::reference_wrapper<CalculatorPair> calculator_;
};

//=====================================================

CalculatorPair::CalculatorPair(const CalculatorBase::Container& c)
    : CalculatorBase(c)
{}

void CalculatorPair::run(ThreadPool& tp)
{
    for (std::size_t i{}; i < size_; ++i)
    {
        tp.addTask(std::make_unique<Worker>(*this));
    }
}


void CalculatorPair::calculate()
{
    if (idx_ == size_) return;
    else
    {
        sum_ += data_.get().at(idx_++);
        // do not return when the last idx reached
        if (idx_ != size_) return;
    }

    result_.set_value(sum_ * 1.0 / size_);
}
