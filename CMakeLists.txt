cmake_minimum_required(VERSION 3.16)
project(thread_pool)

set(CMAKE_CXX_STANDARD 17)

add_executable(thread_pool
        main.cpp
        conc_queue.h
        conc_queue.inl
        task_wrapper.h
        thread_pool.h
        thread_pool.cpp
        utils.h
        tests.h
        tests.cpp

        calculator_base.h

        calculator_pair.h
        calculator_pair.cpp

        calculator_range.h
        calculator_range.cpp)
