cmake_minimum_required(VERSION 3.31)
project(ThreadPool2)

set(CMAKE_CXX_STANDARD 20)

add_executable(ThreadPool2 main.cpp
        worker.cpp
        worker.h
        threadpool.cpp
        threadpool.h)
