#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

namespace qlm
{
	template<typename T>
	class ThreadSafeQueue
	{
	private:
		std::queue<T> data_queue;
		mutable std::mutex mut;
		std::condition_variable cv;
	public:
		ThreadSafeQueue() = default;

		// disable copy and move constructors and the corresponding assignment operators
		ThreadSafeQueue(const ThreadSafeQueue&) = delete;
		ThreadSafeQueue(ThreadSafeQueue&&) = delete;
		ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;
		ThreadSafeQueue& operator=(ThreadSafeQueue&&) = delete;
	public:


	};
}