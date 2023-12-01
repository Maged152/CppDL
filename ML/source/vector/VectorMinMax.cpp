#include "vector.h"
#include <vector>
#include <omp.h>
#include <future>
#include <new>
#include <cmath>
#include <algorithm>

namespace qlm
{
	Status qlm::Vector::MinMax(float& dst_min, float& dst_max, ThreadPool& pool) const
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
		const unsigned int total_length = len;


		auto op_vec = [](const float* const __restrict  src, const unsigned int size)
		{
			std::pair<float, float> dst{ src[0], src[0] };


#pragma omp simd
			for (unsigned int i = 1; i < size; i++)
			{
				dst.first = std::min(dst.first, src[i]);
				dst.second = std::max(dst.second, src[i]);
			}

			return dst;
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
		dst_min = dst.first;
		dst_max = dst.second;

		// wait for the threads to finish
#pragma omp unroll full
		for (unsigned int i = 1; i < num_used_threads; i++)
		{
			auto dst_th = futures[i].get();
			dst_min= std::min(dst_min, dst_th.first);
			dst_max = std::min(dst_max , dst_th.second);

		}

		return Status::SUCCESS;
	}
}