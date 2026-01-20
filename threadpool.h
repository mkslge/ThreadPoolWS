#pragma once

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <random>

#include "worker.h"

inline int DEFAULT_WORKER_SIZE = 20;

class threadpool {
    unsigned seed;
    std::mt19937 mt;

    std::vector<std::unique_ptr<worker>> workers;
    std::vector<std::thread> worker_threads;
    int size;

public:
    threadpool();
    explicit threadpool(int pools);
    ~threadpool();
    threadpool(threadpool &other) = delete;


    bool add_task(const std::function<void()>& task);

    bool try_steal(std::function<void()>* ref);


private:
    void init_threads();
    void init_size(int size);
    void init_rand();

};



#endif //THREADPOOL_H
