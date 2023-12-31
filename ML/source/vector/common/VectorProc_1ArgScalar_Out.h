#include "vector.h"
#include <vector>
#include <omp.h>
#include <future>
#include <utility>

namespace qlm
{
	template <void (*op)(const float, const unsigned int, float&, unsigned int&)>
	inline Status Vector::VectorProc_1ArgScalar_Out(unsigned int& dst, ThreadPool& pool) const
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


		auto op_vec = [](const float* const __restrict  src, const unsigned int start_idx, const unsigned int size) -> std::pair<float, unsigned int>
		{
			float dst = src[0];
			unsigned int dst_idx = start_idx;
#pragma loop( ivdep )
#pragma omp simd
			for (unsigned int i = 1; i < size; i++)
			{
				op(src[i], start_idx + i, dst, dst_idx);
			}

			return std::make_pair(dst, dst_idx);
		};
		// divide the vector among the threads
		const unsigned int thread_length = total_length / num_used_threads;
		const unsigned int thread_tail_length = total_length % num_used_threads;
		std::vector<std::future<std::pair<float, unsigned int>>> futures(num_used_threads);
		// launch the threads
		unsigned int next_idx = 0;

#pragma omp unroll full
		for (unsigned int i = 0; i < thread_tail_length; i++)
		{
			futures[i] = pool.Submit(op_vec, &data[next_idx], next_idx, thread_length + 1);
			next_idx += thread_length + 1;
		}

#pragma omp unroll full
		for (unsigned int i = thread_tail_length; i < num_used_threads; i++)
		{
			futures[i] = pool.Submit(op_vec, &data[next_idx], next_idx, thread_length);
			next_idx += thread_length;
		}

		auto result = futures[0].get();
		float dst_val = result.first;
		dst = result.second;

		// wait for the threads to finish
#pragma omp unroll full
		for (unsigned int i = 1; i < num_used_threads; i++)
		{
			auto result = futures[i].get();
			op(result.first, result.second, dst_val, dst);
		}

		return Status::SUCCESS;
	}
}