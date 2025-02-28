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
	std::future<decltype(std::declval<F>()())> enqueue(F&& f)
	{
		auto task = std::make_shared<std::packaged_task<decltype(f())()>>(std::forward<F>(f));
		std::future<decltype(f())> res = task->get_future();
		{
			std::unique_lock<std::mutex> lock(queue_mutex);
			tasks.emplace([task]()
						{ (*task)(); });
		}
		condition.notify_one();
		return res;
	}

    ~ThreadPool();

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop = false;
};

#endif