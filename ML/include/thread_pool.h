#pragma once

#include "thread_safe_queue.h"
#include <vector>
#include <thread>
#include <functional>
#include <future>
#include <type_traits>

namespace qlm
{
	class ThreadPool
	{
	private:
		std::vector<std::thread> workers;
		ThreadSafeQueue<std::function<void()> > task_queue;

		std::atomic_bool kill;
		std::atomic_bool stop;
		mutable std::mutex mut;
		std::condition_variable cv;

		int thread_count;
		
	private:
		void WorkerThread();

	public:
		ThreadPool(const ThreadPool&) = delete;
		ThreadPool(ThreadPool&&) = delete;
		ThreadPool& operator=(const ThreadPool&) = delete;
		ThreadPool& operator=(ThreadPool&&) = delete;

		ThreadPool(const int thread_count = std::thread::hardware_concurrency());

		~ThreadPool();
	public:
		// number of working threads
		int Size() const;
		// are the threads active
		bool Running() const;
		// stop and process all delegated tasks
		void Stop();
		// stop and drop all tasks remained in queue
		void Kill();
		// submit task
		template <class f, class... Args>
		std::future<std::invoke_result<f(Args...)>> Submit(f&& fun, Args &&...args);
	};


	inline void ThreadPool::WorkerThread()
	{
		while (true)
		{
			std::function<void()> task;

			{
				std::unique_lock lk(mut);
				cv.wait(lk, [this]() {return !task_queue.Empty() || kill || stop; });

				if (kill || (stop && task_queue.Empty()))
				{
					return;
				}
				task_queue.WaitPop(task);
			}

			task();
		}
	}

	inline ThreadPool::ThreadPool(const int thread_count) : thread_count(thread_count), kill(false), stop(false)
	{
		for (int i = 0; i < thread_count; i++) 
		{
			workers.emplace_back(std::bind(& ThreadPool::WorkerThread, this));
		}
	}

	inline ThreadPool::~ThreadPool()
	{
		this->Stop();
	}

	template<class f, class ...Args>
	inline std::future<std::invoke_result<f(Args...)>> ThreadPool::Submit(f&& fun, Args && ...args)
	{
		using return_type = std::invoke_result<f(Args...)>;

		auto task = std::make_shared<std::packaged_task<return_type()>>(
			std::bind(std::forward<f>(fun), std::forward<Args>(args)...));

		std::future<return_type> res = task->get_future();

		{
			std::unique_lock lock(mut);

			if (stop || kill)
				throw std::runtime_error("The thread pool has been stop.");

			task_queue.Push([task]() -> void { (*task)(); });
		}
		cv.notify_one();

		return res;
	}

	inline void ThreadPool::Stop()
	{
		stop = true;
		// notify all threads to finish the remained tasks
		cv.notify_all();
		for (auto& worker : workers)
			worker.join();
	}

	inline void ThreadPool::Kill()
	{
		kill = true;
		// notify all threads to finish the remained tasks
		cv.notify_all();
		for (auto& worker : workers)
			worker.join();
	}

	inline int qlm::ThreadPool::Size() const
	{
		return thread_count;
	}

	inline bool qlm::ThreadPool::Running() const
	{
		std::lock_guard<std::mutex> lk(mut);
		return !stop && !kill;
	}
}