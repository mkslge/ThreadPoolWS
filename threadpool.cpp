//
// Created by Mark on 1/19/26.
//

#include "threadpool.h"


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
    this->size = size;
    workers.reserve(size);
    worker_threads.reserve(size);
}

void threadpool::init_threads() {
    for (int i= 0; i < this->size;i++) {
        workers.emplace_back(std::make_unique<worker>(this));

        worker_threads.emplace_back(&worker::worker_loop, workers.back().get());

    }
}

void threadpool::init_rand() {
    seed = std::chrono::system_clock::now().time_since_epoch().count();
    mt.seed(seed);
}


threadpool::~threadpool() {
    for (int i= 0 ; i < this->size;i++) {
        workers[i].get()->shutdown_worker();
    }

    for (int i= 0 ; i < this->size;i++) {
        worker_threads[i].join();
    }
}

bool threadpool::add_task(const std::function<void()>& task) {
    std::uniform_int_distribution<int> dist(0, this->size - 1);
    int random_index = dist(mt);

    return this->workers[random_index]->add_task(task);
}


bool threadpool::try_steal(std::function<void()>* ref ) {
    for (int i = 0; i < this->size;i++) {
        if (workers[i] == nullptr) {
            break;
        }
        std::pair<bool, std::function<void()>> steal_pair = workers[i]->try_steal();
        if (steal_pair.first) {
            std:: cout << "Work stealing successful." << std::endl;
            *ref = steal_pair.second;
            return true;
        }
    }

    return false;
}



