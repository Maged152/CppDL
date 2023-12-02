#include "vector.h"
#include <vector>
#include <omp.h>
#include <future>
#include <semaphore>
#include <atomic>
#include <cmath>

namespace qlm
{
	Status Vector::Var(float& dst, ThreadPool& pool) const
	{
		float mean;
		const auto status = this->Mean(mean, pool);

		if (status != Status::SUCCESS)
		{
			return status;
		}

		const unsigned int total_length = this->Length();
		const unsigned int num_used_threads = std::min(pool.used_threads, total_length);

		auto var_op = [](const float* const __restrict  src, const float mean,
			const unsigned int size)
		{
			float sum = 0;
#pragma omp simd reduction(+:sum)
			for (unsigned int i = 0; i < size; i++)
			{
				sum += (src[i] - mean) * (src[i] - mean);
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
			futures[i] = pool.Submit(var_op, &data[next_idx], mean, thread_length + 1);
			next_idx += thread_length + 1;
		}

#pragma omp unroll full
		for (unsigned int i = thread_tail_length; i < num_used_threads; i++)
		{
			futures[i] = pool.Submit(var_op, &data[next_idx], mean, thread_length);
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

		return Status::SUCCESS;
	}
}