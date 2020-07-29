cmake_minimum_required(VERSION 3.16)
project(thread_pool)

set(CMAKE_CXX_STANDARD 17)

add_executable(thread_pool
        main.cpp
        conc_queue.h
        conc_queue.inl
        thread_pool.h
        thread_pool.cpp
        utils.h
        tests.h
        tests.cpp

        calculator.h
        calculator.cpp

        )
