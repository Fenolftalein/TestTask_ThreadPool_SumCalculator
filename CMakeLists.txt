cmake_minimum_required(VERSION 3.16)
project(disney_plus)

set(CMAKE_CXX_STANDARD 17)

add_executable(disney_plus
        main.cpp
        conc_queue.h
        conc_queue.inl
        func_wrapper.h
        thread_pool.h
        thread_pool.inl
        thread_pool.cpp
        utils.h
        tests.h
        tests.cpp)
