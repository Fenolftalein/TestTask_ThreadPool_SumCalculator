//
// Created by vladislav.saenko on 28.07.2020.
//

#pragma once

#include "calculator_base.h"

class CalculatorPair : public CalculatorBase
{
public:
    explicit CalculatorPair(const Container& c);

    void run(ThreadPool& tp) override;

private:
    class Worker;
    friend class Worker;
    // Method providing synchronization: called implicitly by workers
    void calculate();

    std::atomic<std::size_t> idx_{};
};
