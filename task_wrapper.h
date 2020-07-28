//
// Created by vladislav.saenko on 27.07.2020.
//

#pragma once

#include <memory>

// simple task-wrapper
struct Task
{
    virtual ~Task() = default;
    virtual void operator()() = 0;
};

using TaskPtr = std::unique_ptr<Task>;
