#include "vector.h"
#include <vector>
#include <omp.h>
#include <future>
#include <new>
#include <cmath>
#include <algorithm>

namespace qlm
{
	template <const float& (*op)(const float&, const float&)>
	inline Status Vector::VectorOp(float& dst, ThreadPool& pool) const
	{
		if (pool.used_threads <= 0)
		{
			return Status::INVALID_UTILIZATION;
		}

		if (len < 1)
		{
			return Status::INVALID_DIMENTIONS;
		}

		const unsigned int num_used_threads = pool.used_threads;
		const unsigned int total_length =len;


		auto op_vec = [](const float* const __restrict  src, const unsigned int size)
		{
			float dst = src[0];
#pragma loop( ivdep )
#pragma omp simd
			for (unsigned int i = 0; i < size; i++)
			{
				dst = op(dst, src[i]);
			}

			return dst;
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
			dst = op(dst, futures[i].get());
		}

		return Status::SUCCESS;
	}


	template Status Vector::VectorOp<std::min>(float&, ThreadPool&) const;

	template Status Vector::VectorOp<std::max>(float&, ThreadPool&) const;
}