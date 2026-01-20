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




int main() {
    threadpool tp = threadpool(5);
    for (int i = 0 ; i < 10;i++) {
        tp.add_task(fakeTask(i));
    }


    return 0;
}
