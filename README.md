***Thread Pool Implementation***

This is my implementation of a threadpool with work-stealing functionality.

**Comparison to single-threaded application**

*Function:*

`
void fakeTask(int id) {
    std::cout << "Started task, ID: " << id << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "Ended Task, ID: " << id << std::endl;
}
`

*Single threaded code:*
`
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    for (int i= 0 ; i < 10;i++) {
        std::function<void()> task = fakeTask(i);
        task();
    }
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> dur = end - start;
    double seconds = dur.count() / 1000;
    std::cout << "Time taken: " << seconds << " seconds" << std::endl;
`

Output:
`
Time taken: 30.0486 seconds
`

*Thead pool code:*

`
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    threadpool tp = threadpool(5);
    for (int i = 0 ; i < 10;i++) {
        tp.add_task(fakeTask(i));
    }
    tp.~threadpool();
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> dur = end - start;
    double seconds = dur.count() / 1000;
    std::cout << "Time taken: " << seconds << " seconds" << std::endl;
`

Output:
`
Time taken: 6.00654 seconds
`