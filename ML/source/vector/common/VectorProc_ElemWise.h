#include "vector.h"
#include <vector>
#include <omp.h>
#include <future>

namespace qlm
{
	template<float (*op)(const float, const float)>
	inline Status Vector::VectorProc_ElemWise(const Vector& src, Vector& dst, ThreadPool& pool) const
	{
		if (pool.used_threads <= 0)
		{
			return Status::INVALID_UTILIZATION;
		}

		if (src.Length() != dst.Length() || src.Length() != len)
		{
			return Status::INVALID_DIMENTIONS;
		}

		const unsigned int total_length = len;
		const unsigned int lines = std::ceil((float)len / std::hardware_destructive_interference_size);
		const unsigned int threads = std::min(lines, pool.used_threads);


		auto op_vec = [](const float* const __restrict  src1, const float* const __restrict  src2, float* const __restrict  dst, const unsigned int size)
		{
#pragma loop( ivdep )
#pragma omp simd
			for (unsigned int i = 0; i < size; i++)
			{
				dst[i] = op(src1[i], src2[i]);
			}
		};
		// divide the matrix among the threads
		const unsigned int thread_len = (lines / threads) * std::hardware_destructive_interference_size;
		const unsigned int thread_tail = total_length - thread_len * threads;
		std::vector<std::future<void>> futures(threads);
		// launch the threads
		int next_idx = 0;

#pragma omp unroll full
		for (unsigned int i = 0; i < threads - 1; i++)
		{
			futures[i] = pool.Submit(op_vec, &data[next_idx], &src.data[next_idx], &dst.data[next_idx], thread_len);
			next_idx += thread_len;
		}
		// tail thread
		futures[threads - 1] = pool.Submit(op_vec, &data[next_idx], &src.data[next_idx], &dst.data[next_idx], thread_len + thread_tail);

		// wait for the threads to finish
#pragma omp unroll full
		for (unsigned int i = 0; i < threads; i++)
		{
			futures[i].wait();
		}

		return Status::SUCCESS;
	}

	/********************************************************************************************************/
	
	template<float (*op)(const float, const float)>
	inline Status Vector::VectorProc_ElemWise(const float val, Vector& dst, ThreadPool& pool) const
	{
		if (pool.used_threads <= 0)
		{
			return Status::INVALID_UTILIZATION;
		}

		if (dst.Length() != len)
		{
			return Status::INVALID_DIMENTIONS;
		}

		const unsigned int total_length = len;
		const unsigned int num_used_threads = std::min(pool.used_threads, total_length);

		auto op_vec = [](const float* const __restrict  src1, const float val, float* const __restrict  dst, const unsigned int size)
		{
#pragma loop( ivdep )
#pragma omp simd
			for (unsigned int i = 0; i < size; i++)
			{
				dst[i] = op(src1[i], val);
			}
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
			futures[i] = pool.Submit(op_vec, &data[next_idx], val, &dst.data[next_idx], thread_length + 1);
			next_idx += thread_length + 1;
		}

#pragma omp unroll full
		for (unsigned int i = thread_tail_length; i < num_used_threads; i++)
		{
			futures[i] = pool.Submit(op_vec, &data[next_idx], val, &dst.data[next_idx], thread_length);
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

	/********************************************************************************************************/

	template<float (*op)(const float)>
	inline Status Vector::VectorProc_ElemWise(Vector& dst, ThreadPool& pool) const
	{
		if (pool.used_threads <= 0)
		{
			return Status::INVALID_UTILIZATION;
		}

		if (dst.Length() != len)
		{
			return Status::INVALID_DIMENTIONS;
		}

		const unsigned int total_length = len;
		const unsigned int num_used_threads = std::min(pool.used_threads, total_length);

		auto op_vec = [](const float* const __restrict  src, float* const __restrict  dst, const unsigned int size)
		{
#pragma loop( ivdep )
#pragma omp simd
			for (unsigned int i = 0; i < size; i++)
			{
				dst[i] = op(src[i]);
			}
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
			futures[i] = pool.Submit(op_vec, &data[next_idx], &dst.data[next_idx], thread_length + 1);
			next_idx += thread_length + 1;
		}

#pragma omp unroll full
		for (unsigned int i = thread_tail_length; i < num_used_threads; i++)
		{
			futures[i] = pool.Submit(op_vec, &data[next_idx], &dst.data[next_idx], thread_length);
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