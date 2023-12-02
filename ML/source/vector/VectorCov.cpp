#include "vector.h"
#include <vector>
#include <omp.h>
#include <future>
#include <semaphore>
#include <atomic>
#include <cmath>

namespace qlm
{
	Status Vector::Cov(const Vector& src, float& dst, ThreadPool& pool) const
	{
		const int old_pool_size = pool.used_threads;
		const int new_pool_size = std::ceil(old_pool_size / 2);
		pool.used_threads = new_pool_size;

		float mean1, mean2;

		auto bind_mean1 = [&]() { return this->Mean(mean1, pool); };
		auto bind_mean2 = [&]() { return src.Mean(mean2, pool); };

		auto fut_status1 = pool.Submit(bind_mean1);
		auto fut_status2 = pool.Submit(bind_mean2);

		auto status1 = fut_status1.get();
		if (status1 != Status::SUCCESS)
		{
			return status1;
			pool.used_threads = old_pool_size;
		}

		auto status2 = fut_status2.get();
		if (status2 != Status::SUCCESS)
		{
			return status2;
			pool.used_threads = old_pool_size;
		}

		const unsigned int num_used_threads = pool.used_threads;
		const unsigned int total_length = this->Length();

		auto cov_op = [](const float* const __restrict  src1, const float mean1,
						 const float* const __restrict  src2, const float mean2,
			const unsigned int size)
		{
			float sum = 0;
#pragma omp simd reduction(+:sum)
			for (unsigned int i = 0; i < size; i++)
			{
				sum += (src1[i] - mean1) * (src2[i] - mean2);
			}
			return sum;
		};
		// divide the matrix among the threads
		const unsigned int thread_length = total_length / num_used_threads;
		const unsigned int thread_tail_length = total_length % num_used_threads;
		std::vector<std::future<float>> futures(num_used_threads);
		// launch the threads
		int next_idx = 0;

#pragma omp unroll full
		for (unsigned int i = 0; i < thread_tail_length; i++)
		{
			futures[i] = pool.Submit(cov_op, &data[next_idx], mean1, &src.data[next_idx], mean2, thread_length + 1);
			next_idx += thread_length + 1;
		}

#pragma omp unroll full
		for (unsigned int i = thread_tail_length; i < num_used_threads; i++)
		{
			futures[i] = pool.Submit(cov_op, &data[next_idx], mean1, &src.data[next_idx], mean2, thread_length);
			next_idx += thread_length;
		}
		// ensuring sum variable is 0
		dst = 0;
		// wait for the threads to finish
#pragma omp unroll full
		for (unsigned int i = 0; i < num_used_threads; i++)
		{
			dst += futures[i].get();
		}

		dst = dst / (len - 1);

		pool.used_threads = old_pool_size;

		return Status::SUCCESS;
	}
}