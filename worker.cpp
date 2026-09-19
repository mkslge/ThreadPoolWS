//
// Created by Mark on 1/19/26.
//

#include "worker.h"

#include "threadpool.h"


worker::worker(threadpool* parent_pool_ref, int idx) {
    this->parent_pool_ref = parent_pool_ref;
    this->idx = idx;

}

worker::~worker() {
}

void worker::worker_loop() {
    while (true) {
        std::size_t observed_generation;
        {
            std::lock_guard<std::mutex> guard(parent_pool_ref->work_mutex);
            observed_generation = parent_pool_ref->work_generation;
        }

        std::function<void()> func;
        bool found_task = false;
        {
            std::lock_guard<std::mutex> guard(lock);
            if (!tasks.empty()) {
                func = std::move(tasks.front());
                tasks.pop_front();
                found_task = true;
            }
        }

        if (!found_task) {
            found_task = parent_pool_ref->try_steal(&func, idx);
        }

        if (!found_task) {
            std::unique_lock<std::mutex> guard(parent_pool_ref->work_mutex);
            if (parent_pool_ref->shutting_down) {
                return;
            }

            parent_pool_ref->work_cv.wait(guard, [this, observed_generation] {
                return parent_pool_ref->shutting_down ||
                       parent_pool_ref->work_generation != observed_generation;
            });
            continue;
        }

        try {
            func();
        } catch (...) {

        }

    }
}

bool worker::empty() {
    std::lock_guard<std::mutex> lg(lock);
    return tasks.empty();
}

bool worker::add_task(const std::function<void()>& task) {
    std::lock_guard<std::mutex> lg(lock);
    tasks.push_front(task);
    return true;
}

std::optional< std::function<void()>> worker::try_steal() {

    std::lock_guard<std::mutex> lg(lock);
    if (this->tasks.empty()) {
        return std::nullopt;
    } else {
        std::function<void()> stolen_func = std::move(this->tasks.back());
        this->tasks.pop_back();
        return {stolen_func};
    }

}
