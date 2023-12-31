#include "vector.h"
#include <vector>
#include <omp.h>
#include <future>
#include <utility>
#include <tuple>

namespace qlm
{
	template <void (*op)(const float, const unsigned int, float&, unsigned int&, float&, unsigned int&)>
	inline Status Vector::VectorProc_2ArgScalar_Out(unsigned int& dst0, unsigned int& dst1, ThreadPool& pool) const
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


		auto op_vec = [](const float* const __restrict  src, const unsigned int start_idx, const unsigned int size)
		{
			float dst0 = src[0];
			unsigned int dst_idx0 = start_idx;

			float dst1 = src[0];
			unsigned int dst_idx1 = start_idx;

#pragma loop( ivdep )
#pragma omp simd
			for (unsigned int i = 1; i < size; i++)
			{
				op(src[i], start_idx + i, dst0, dst_idx0, dst1, dst_idx1);
			}

			return std::make_tuple(dst0, dst_idx0, dst1, dst_idx1);
		};
		// divide the vector among the threads
		const unsigned int thread_length = total_length / num_used_threads;
		const unsigned int thread_tail_length = total_length % num_used_threads;
		std::vector<std::future<std::tuple<float, unsigned int, float, unsigned int>>> futures(num_used_threads);
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
		float dst0_val = std::get<0>(result);
		dst0 = std::get<1>(result);

		float dst1_val = std::get<2>(result);
		dst1 = std::get<3>(result);

		// wait for the threads to finish
#pragma omp unroll full
		for (unsigned int i = 1; i < num_used_threads; i++)
		{
			auto result = futures[i].get();
			op(std::get<0>(result), std::get<1>(result), dst0_val, dst0, dst1_val, dst1);
			op(std::get<2>(result), std::get<3>(result), dst0_val, dst0, dst1_val, dst1);
		}

		return Status::SUCCESS;
	}
}