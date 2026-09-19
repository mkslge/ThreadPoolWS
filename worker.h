#pragma once

#ifndef WORKER_H
#define WORKER_H


#include <iostream>
#include <mutex>
#include <thread>
#include <deque>
#include <functional>
#include <utility>
#include <optional>


//forward declaration here to avoid infinite #include
class threadpool;

class worker {
    threadpool* parent_pool_ref;
    std::mutex lock = std::mutex();
    std::deque<std::function<void()>> tasks;

    int idx;
public:
    worker(threadpool* parent_pool_ref, int idx);
    ~worker();
    worker( worker &other) = delete;
    worker& operator=(const worker& other) = delete;
    worker&& operator=(worker& other ) = delete;

    void worker_loop();

    bool empty();
    //add to stack (front), and steal from queue(back) to avoid overlapping shared resources amap


    bool add_task(const std::function<void()>&);
    std::optional <std::function<void()>> try_steal();


};



#endif //WORKER_H
