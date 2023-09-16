#include "vector.h"
#include "config.h"
#include <vector>
#include <omp.h>
#include <future>
#include <semaphore>

namespace qlm
{
	Status Vector::Dot(const Vector& src, float& dst, float utilization)
	{
		if (utilization <= 0)
		{
			return Status::INVALID_UTILIZATION;
		}

		if (src.Length() != len)
		{
			return Status::INVALID_DIMENTIONS;
		}

		const unsigned int num_used_threads = utilization > 1.0f ? static_cast<unsigned int>(utilization) : static_cast<unsigned int>(utilization * num_threads);
		const unsigned int total_length = len;
		std::binary_semaphore sem {1};

		auto dot_op = [&](const float* const __restrict  src1,
						 const float* const __restrict  src2, 
						 float& dst,
						 const unsigned int size)
		{
			float sum = 0;
			#pragma omp simd reduction(+:sum)
			for (unsigned int i = 0; i < size; i++)
			{
				sum += src1[i] * src2[i];
			}
			// critical section
			sem.acquire();
			dst += sum;
			sem.release();
		};
		// divide the matrix among the threads
		const unsigned int thread_length = total_length / num_used_threads;
		const unsigned int thread_tail_length = total_length % num_used_threads;
		std::vector<std::future<void>> futures(num_used_threads);
		// launch the threads
		int next_idx = 0;

#pragma omp unroll full
		for (unsigned int i = 0; i < thread_tail_length; i++)
		{
			futures[i] = std::async(std::launch::async, dot_op, &data[next_idx], &src.data[next_idx], std::ref(dst), thread_length + 1);
			next_idx += thread_length + 1;
		}

#pragma omp unroll full
		for (unsigned int i = thread_tail_length; i < num_used_threads; i++)
		{
			futures[i] = std::async(std::launch::async, dot_op, &data[next_idx], &src.data[next_idx], std::ref(dst), thread_length);
			next_idx += thread_length;
		}
		// wait for the threads to finish
#pragma omp unroll full
		for (unsigned int i = 0; i < num_used_threads; i++)
		{
			futures[i].wait();
		}

		return Status::SUCCESS;
	}
}