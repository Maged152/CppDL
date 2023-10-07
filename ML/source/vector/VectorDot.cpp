#include "vector.h"
#include <vector>
#include <omp.h>
#include <future>
#include <semaphore>
#include <atomic>

namespace qlm
{
	Status Vector::Dot(const Vector& src, float& dst, ThreadPool& pool)
	{
		if (pool.Size() <= 0)
		{
			return Status::INVALID_UTILIZATION;
		}

		if (src.Length() != this->Length())
		{
			return Status::INVALID_DIMENTIONS;
		}

		const unsigned int num_used_threads = pool.Size();
		const unsigned int total_length = src.Length();

		auto dot_op = [](const float* const __restrict  src1,
						 const float* const __restrict  src2, 
						 const unsigned int size)
		{
			float sum = 0;
			#pragma omp simd reduction(+:sum)
			for (unsigned int i = 0; i < size; i++)
			{
				sum += src1[i] * src2[i];
			}
			// critical section
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
			futures[i] = pool.Submit(dot_op, &data[next_idx], &src.data[next_idx], thread_length + 1);
			next_idx += thread_length + 1;
		}

#pragma omp unroll full
		for (unsigned int i = thread_tail_length; i < num_used_threads; i++)
		{
			futures[i] = pool.Submit(dot_op, &data[next_idx], &src.data[next_idx], thread_length);
			next_idx += thread_length;
		}
		// wait for the threads to finish
		pool.Stop();
#pragma omp unroll full
		for (unsigned int i = 0; i < num_used_threads; i++)
		{
			dst += futures[i].get();
		}

		return Status::SUCCESS;
	}
}