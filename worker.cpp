//
// Created by Mark on 1/19/26.
//

#include "worker.h"


worker::worker() {
    shutdown = false;
}

worker::~worker() = default;

void worker::worker_loop() {
    while (true) {
        std::function<void()> func;
        {
            std::cout << "Looping..." << std::endl;
            std::unique_lock<std::mutex> ul(lock);

            cv.wait( ul, [this]  {
                return this->shutdown || !this->tasks.empty();
            });

            if (this->tasks.empty() && this->shutdown) {
                return;
            }

            func = std::move(tasks.front());
            tasks.pop_front();
        }
        func();
    }
}

bool worker::empty() {
    std::lock_guard<std::mutex> lg(lock);
    std::cout << "Checking if empty, size: " << tasks.size() << std::endl;
    return tasks.empty();
}

bool worker::add_task(const std::function<void()>& task) {
    std::lock_guard<std::mutex> lg(lock);
    tasks.push_front(task);
    std::cout << "Pushing task..." << std::endl;
    return true;
}

std::pair<bool, std::function<void()>> worker::try_steal() {

    std::lock_guard<std::mutex> lg(lock);
    if (this->tasks.empty()) {
        return std::pair {false, nullptr };
    } else {
        std::function<void()> stolen_func = std::move(this->tasks.back());
        this->tasks.pop_back();
        return std::pair {true, stolen_func};
    }

}

void worker::shutdown_worker() {
    std::lock_guard<std::mutex> lg(lock);
    shutdown = true;
}


