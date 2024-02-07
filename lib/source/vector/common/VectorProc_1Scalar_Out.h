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
			return Status::INVALID_DIMENSIONS;
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

	/***********************************************************************************************/

	template<float(*op)(const float, const float, const float)>
	inline Status qlm::Vector::VectorProc_1Scalar_Out(const Vector& src, float& dst, ThreadPool& pool) const
	{
		if (pool.used_threads <= 0)
		{
			return Status::INVALID_UTILIZATION;
		}

		if (src.Length() != this->Length())
		{
			return Status::INVALID_DIMENSIONS;
		}

		const unsigned int total_length = src.Length();
		const unsigned int num_used_threads = std::min(pool.used_threads, total_length);

		auto dot_op = [](const float* const __restrict  src1,
			const float* const __restrict  src2,
			const unsigned int size)
		{
			float sum = 0;
#pragma omp simd reduction(+:sum)
			for (unsigned int i = 0; i < size; i++)
			{
				sum = op(src1[i], src2[i], sum);
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