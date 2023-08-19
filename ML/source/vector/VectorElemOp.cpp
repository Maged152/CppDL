#include "vector.h"
#include "config.h"
#include <vector>
#include <omp.h>
#include <future>

namespace qlm
{
	template<typename op>
	inline Status Vector::VectorElemOp(const float val, Vector& dst, float utilization)
	{
		if (utilization <= 0)
		{
			return Status::INVALID_UTILIZATION;
		}

		if (dst.Length() != len)
		{
			return Status::INVALID_DIMENTIONS;
		}

		const unsigned int num_used_threads = utilization > 1.0f ? static_cast<unsigned int>(utilization) : static_cast<unsigned int>(utilization * num_threads);
		const unsigned int total_length = len;

		auto op_vec = [](const float* const __restrict  src1, const float val, float* const __restrict  dst, const unsigned int size)
		{
#pragma loop( ivdep )
#pragma omp simd
			for (unsigned int i = 0; i < size; i++)
			{
				dst[i] = op()(src1[i], val);
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
			futures[i] = std::async(std::launch::async, op_vec, &data[next_idx], val, &dst.data[next_idx], thread_length + 1);
			next_idx += thread_length + 1;
		}

#pragma omp unroll full
		for (unsigned int i = thread_tail_length; i < num_used_threads; i++)
		{
			futures[i] = std::async(std::launch::async, op_vec, &data[next_idx], val, &dst.data[next_idx], thread_length);
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


	template Status Vector::VectorElemOp<std::plus<float>>(const float, Vector&, float);
	template Status Vector::VectorElemOp<std::minus<float>>(const float, Vector&, float);
	template Status Vector::VectorElemOp<std::multiplies<float>>(const float, Vector&, float);
	template Status Vector::VectorElemOp<std::divides<float>>(const float, Vector&, float);
}