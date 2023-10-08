#include "vector.h"
#include <vector>
#include <omp.h>
#include <future>
#include <new>
#include <cmath>

namespace qlm
{
	template<typename op>
	inline Status Vector::VectorElemWiseOp(const Vector& src, Vector& dst, ThreadPool& pool)
	{
		if (pool.Size() <= 0)
		{
			return Status::INVALID_UTILIZATION;
		}

		if (src.Length() != dst.Length() || src.Length() != len)
		{
			return Status::INVALID_DIMENTIONS;
		}

		const unsigned int total_length = len;
		const  int lines = std::ceil((float)len / std::hardware_destructive_interference_size);
		const  int threads =  std::min(lines, pool.Size());
		

		auto op_vec = [](const float* const __restrict  src1, const float* const __restrict  src2, float* const __restrict  dst, const unsigned int size)
		{
#pragma loop( ivdep )
#pragma omp simd
			for (unsigned int i = 0; i < size; i++)
			{
				dst[i] = op()(src1[i], src2[i]);
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


	template Status Vector::VectorElemWiseOp<std::plus<float>>(const Vector&, Vector&, ThreadPool&);
	template Status Vector::VectorElemWiseOp<std::minus<float>>(const Vector&, Vector&, ThreadPool&);
	template Status Vector::VectorElemWiseOp<std::multiplies<float>>(const Vector&, Vector&, ThreadPool&);
	template Status Vector::VectorElemWiseOp<std::divides<float>>(const Vector&, Vector&, ThreadPool&);
}