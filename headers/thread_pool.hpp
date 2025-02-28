#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <vector>
#include <future>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

class ThreadPool {
public:
    ThreadPool(size_t num_threads);

    template <class F>
    std::future<decltype(std::declval<F>)> enqueue(F&& f);

    ~ThreadPool();

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop = false;
};

#endif