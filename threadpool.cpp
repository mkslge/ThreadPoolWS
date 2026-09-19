//
// Created by Mark on 1/19/26.
//

#include "threadpool.h"
#include <optional>


threadpool::threadpool() {
    this->init_rand();
    this->init_size(DEFAULT_WORKER_SIZE);
    this->init_threads();
}


threadpool::threadpool(int pools) {
    this->init_rand();
    this->init_size(pools);
    this->init_threads();
}


void threadpool::init_size(int size) {
    if(size <= 0) {
        size = DEFAULT_WORKER_SIZE;
    }
    this->size = size;
    workers.reserve(size);
    worker_threads.reserve(size);
}

void threadpool::init_threads() {
    for (auto i= 0; i < this->size;i++) {
        workers.emplace_back(std::make_unique<worker>(this, i));
    }

    try {
        for(auto i = 0; i < this->size;i++) {
            worker_threads.emplace_back(&worker::worker_loop, workers[i].get());
        }
    } catch (...) {
        {
            std::lock_guard<std::mutex> guard(work_mutex);
            shutting_down = true;
        }
        work_cv.notify_all();

        for (auto& thread : worker_threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }

        throw;
    }
}

void threadpool::init_rand() {
    seed = std::chrono::system_clock::now().time_since_epoch().count();
    mt.seed(seed);
}


threadpool::~threadpool() {
    {
        std::lock_guard<std::mutex> guard(work_mutex);
        shutting_down = true;
    }
    work_cv.notify_all();

    for (int i= 0 ; i < this->size;i++) {
        worker_threads[i].join();
    }
}

bool threadpool::add_task(const std::function<void()>& task) {
    int random_index;
    {
        std::lock_guard<std::mutex> guard(mt_mutex);
        std::uniform_int_distribution<int> dist(0, this->size - 1);
        random_index = dist(mt);
    }


    {
        std::lock_guard<std::mutex> guard(work_mutex);
        if (shutting_down) {
            return false;
        }

        this->workers[random_index]->add_task(task);
        ++work_generation;
    }

    work_cv.notify_all();
    return true;
}


bool threadpool::try_steal(std::function<void()>* ref, int idx) {
    for (int i = 0; i < this->size;i++) {
        if(idx == i) {
            continue;
        }
        if (workers[i] == nullptr) {
            break;
        }
        auto  stolen_func = workers[i]->try_steal();
        if (stolen_func.has_value()) {
            *ref = stolen_func.value();
            return true;
        }
    }

    return false;
}
