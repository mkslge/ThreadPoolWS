//
// Created by Mark on 1/19/26.
//

#ifndef WORKER_H
#define WORKER_H

#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <deque>
#include <functional>
#include <utility>

class worker {
    std::mutex lock = std::mutex();
    std::condition_variable cv;
    std::deque<std::function<void()>> tasks;

    bool shutdown;

public:
    worker();
    ~worker();
    worker( worker &other) = delete;
    worker& operator=(const worker& other) = delete;
    worker&& operator=(worker& other ) = delete;

    void worker_loop();
    void shutdown_worker();

    bool empty();
    //add to stack (front), and steal from queue(back) to avoid overlapping shared resources amap


    bool add_task(const std::function<void()>&);
    std::pair <bool, std::function<void()>> try_steal();


};



#endif //WORKER_H
