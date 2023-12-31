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
	template <void (*op)(const float, float&, float&)>
	inline Status Vector::VectorProc_2Scalar_Out(float& dst0, float& dst1, ThreadPool& pool) const
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
			float dst0 = src[0];
			float dst1 = src[0];

#pragma omp simd
			for (unsigned int i = 1; i < size; i++)
			{
				op(src[i], dst0, dst1);
			}

			return std::make_pair(dst0, dst1);
		};
		// divide the matrix among the threads
		const unsigned int thread_length = total_length / num_used_threads;
		const unsigned int thread_tail_length = total_length % num_used_threads;
		std::vector<std::future<std::pair<float, float>>> futures(num_used_threads);
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

		auto dst = futures[0].get();
		dst0 = dst.first;
		dst1 = dst.second;

		// wait for the threads to finish
#pragma omp unroll full
		for (unsigned int i = 1; i < num_used_threads; i++)
		{
			auto dst_th = futures[i].get();
			op(dst_th.first, dst0, dst1);
			op(dst_th.second, dst0, dst1);
		}

		return Status::SUCCESS;
	}
}