//
// Created by Mark on 1/19/26.
//

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include "worker.h"

inline int WORKER_SIZE = 20;

class threadpool {
    std::vector<worker>(WORKER_SIZE);

public:
    threadpool();
    ~threadpool();
    threadpool(threadpool &other) = delete;

};



#endif //THREADPOOL_H
