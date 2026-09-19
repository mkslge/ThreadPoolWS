cmake_minimum_required(VERSION 3.31)
project(ThreadPool2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(threadpool
        threadpool.cpp
        worker.cpp)
target_include_directories(threadpool PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(ThreadPoolDemo main.cpp)
target_link_libraries(ThreadPoolDemo PRIVATE threadpool)

add_executable(ThreadPoolBenchmark benchmarks/threadpool_benchmark.cpp)
target_link_libraries(ThreadPoolBenchmark PRIVATE threadpool)
target_compile_definitions(ThreadPoolBenchmark PRIVATE
        THREADPOOL_BUILD_TYPE="$<IF:$<CONFIG:Debug>,Debug,Release>")

include(CTest)
if(BUILD_TESTING)
    add_executable(ThreadPoolTests tests/threadpool_tests.cpp)
    target_link_libraries(ThreadPoolTests PRIVATE threadpool)
    add_test(NAME ThreadPoolTests COMMAND ThreadPoolTests)
    set_tests_properties(ThreadPoolTests PROPERTIES TIMEOUT 15)
endif()
