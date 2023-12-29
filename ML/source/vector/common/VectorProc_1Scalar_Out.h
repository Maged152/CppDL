#pragma once

#include "vector.h"
#include <vector>
#include <omp.h>
#include <future>
#include <new>
#include <cmath>
#include <algorithm>


namespace qlm
{
	template <float (*op)(const float, const float)>
	inline Status Vector::VectorProc_1Scalar_Out(float& dst, ThreadPool& pool) const
	{
		if (pool.used_threads <= 0)
		{
			return Status::INVALID_UTILIZATION;
		}

		if (len < 1)
		{
			return Status::INVALID_DIMENTIONS;
		}

		const unsigned int total_length = len;
		const unsigned int num_used_threads = std::min(pool.used_threads, total_length);


		auto op_vec = [](const float* const __restrict  src, const unsigned int size)
		{
			float dst = src[0];
#pragma loop( ivdep )
#pragma omp simd
			for (unsigned int i = 1; i < size; i++)
			{
				dst = op(src[i], dst);
			}

			return dst;
		};
		// divide the vector among the threads
		const unsigned int thread_length = total_length / num_used_threads;
		const unsigned int thread_tail_length = total_length % num_used_threads;
		std::vector<std::future<float>> futures(num_used_threads);
		// launch the threads
		int next_idx = 0;

#pragma omp unroll full
		for (unsigned int i = 0; i < thread_tail_length; i++)
		{
			futures[i] = pool.Submit(op_vec, &data[next_idx], thread_length + 1);
			next_idx += thread_length + 1;
		}

#pragma omp unroll full
		for (unsigned int i = thread_tail_length; i < num_used_threads; i++)
		{
			futures[i] = pool.Submit(op_vec, &data[next_idx], thread_length);
			next_idx += thread_length;
		}

		dst = futures[0].get();
		// wait for the threads to finish
#pragma omp unroll full
		for (unsigned int i = 1; i < num_used_threads; i++)
		{
			dst = op(futures[i].get(), dst);
		}

		return Status::SUCCESS;
	}
}