#include "vector.h"
#include <vector>
#include <omp.h>
#include <future>
#include <semaphore>
#include <atomic>

namespace qlm
{
	Status Vector::Sum(float& dst, ThreadPool& pool) const
	{
		if (pool.used_threads <= 0)
		{
			return Status::INVALID_UTILIZATION;
		}

		if (this->Length() <= 0)
		{
			return Status::INVALID_DIMENTIONS;
		}

		const unsigned int total_length = this->Length();
		const unsigned int num_used_threads = std::min(pool.used_threads, total_length);

		auto sum_op = [](const float* const __restrict  src,
						 const unsigned int size)
		{
			float sum = 0;
#pragma omp simd reduction(+:sum)
			for (unsigned int i = 0; i < size; i++)
			{
				sum += src[i];
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
			futures[i] = pool.Submit(sum_op, &data[next_idx], thread_length + 1);
			next_idx += thread_length + 1;
		}

#pragma omp unroll full
		for (unsigned int i = thread_tail_length; i < num_used_threads; i++)
		{
			futures[i] = pool.Submit(sum_op, &data[next_idx], thread_length);
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

		return Status::SUCCESS;
	}
}