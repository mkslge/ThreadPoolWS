#include <iostream>
#include <chrono>

#include "worker.h"
#include "threadpool.h"
std::function<void()> fakeTask(int id) {

    return [id]()  {
        std::cout << "Started task, ID: " << id << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout << "Ended Task, ID: " << id << std::endl;
    };
}

void thread_pool_approach(int num_tasks) {
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    threadpool tp = threadpool(5);
    for (int i = 0 ; i < num_tasks;i++) {
        tp.add_task(fakeTask(i));
    }

    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> dur = end - start;

    double seconds = dur.count() / 1000;
    std::cout << "Time taken: " << seconds << " seconds" << std::endl;
}

void single_threaded_approach(int num_tasks) {
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    for (int i= 0 ; i < num_tasks;i++) {
        std::function<void()> task = fakeTask(i);
        task();
    }

    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> dur = end - start;

    double seconds = dur.count() / 1000;
    std::cout << "Time taken: " << seconds << " seconds" << std::endl;
}

int main() {
    thread_pool_approach(10);
    return 0;
}
