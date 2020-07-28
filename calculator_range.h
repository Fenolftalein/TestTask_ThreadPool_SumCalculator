//
// Created by vladislav.saenko on 29.07.2020.
//

#pragma once

#include "calculator_base.h"

class CalculatorRange : public CalculatorBase
{
public:
    explicit CalculatorRange(const Container& c);
    void run(ThreadPool& tp) override;

    using IterType = typename Container::const_iterator;
private:
    class Worker;
    friend class Worker;

    void calculate(IterType first, IterType last);

    std::atomic_bool finished_{};
};
