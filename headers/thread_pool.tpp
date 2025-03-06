#include "thread_pool.hpp"

template <class F>
std::future<decltype(std::declval<F>()())> ThreadPool::enqueue(F&& f)
{
	auto task = std::make_shared<std::packaged_task<decltype(f())()>>(std::forward<F>(f));
	std::future<decltype(f())> res = task->get_future();
	{
		std::unique_lock<std::mutex> lock(queue_mutex);
		tasks.emplace([task](){ (*task)(); });
	}
	condition.notify_one();
	return res;
}