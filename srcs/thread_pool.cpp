#include "thread_pool.hpp"

ThreadPool::ThreadPool(size_t num_threads) {
	for (size_t i = 0; i < num_threads; ++i) {
		workers.emplace_back([this] {
			while (true) {
				std::function<void()> task;
				{
					std::unique_lock<std::mutex> lock(this->queue_mutex);
					this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });
					if (this->stop && this->tasks.empty()) return;
					task = std::move(this->tasks.front());
					this->tasks.pop();
				}
				task();
			}
		});
	}
}

template <class F>
std::future<decltype(std::declval<F>)> ThreadPool::enqueue(F &&f)
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

ThreadPool::~ThreadPool() {
	{
		std::unique_lock<std::mutex> lock(queue_mutex);
		stop = true;
	}
	condition.notify_all();
	for (std::thread &worker : workers)
		worker.join();
}