#include <iostream>
#include <chrono>

#include "worker.h"

std::function<void()> fakeTask(int id) {

    return [id]()  {
        std::cout << "Started task, ID: " << id << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout << "Ended Task, ID: " << id << std::endl;
    };
}

int main() {
    worker test_worker = worker();

    std::thread thread1(&worker::worker_loop, &test_worker);

    test_worker.add_task(fakeTask(1));
    test_worker.add_task(fakeTask(2));
    test_worker.add_task(fakeTask(3));

    test_worker.try_steal();


    test_worker.shutdown_worker();
    thread1.join();
    return 0;
}
