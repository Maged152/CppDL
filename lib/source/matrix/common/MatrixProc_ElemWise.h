#pragma once

#include "matrix.h"
#include <vector>
#include <omp.h>
#include <future>

namespace qlm
{
    // two input matrix, one output matrix
    template<auto op, typename... Args>
    inline Status Matrix::MatrixProc_ElemWise(const Matrix& src, Matrix& dst, ThreadPool& pool, Args... args) const
    {
        if (pool.used_threads <= 0)
		{
			return Status::INVALID_UTILIZATION;
		}

		if (src.Columns() != dst.Columns() || src.Rows() != dst.Rows() || src.Columns() != this->Columns() || src.Rows() != this->Rows())
		{
			return Status::INVALID_DIMENSIONS;
		}

		const size_t total_length = rows * columns;
        const size_t num_used_threads = std::min(pool.used_threads, total_length);

        auto op_mat = [&](const float* const __restrict  src1, const float* const __restrict  src2, float* const __restrict  dst, const size_t size)
		{
            #pragma loop( ivdep )
            #pragma omp simd
			for (size_t i = 0; i < size; i++)
			{
				dst[i] = op(src1[i], src2[i], args...);
			}
		};

        // divide the matrix among the threads
		const size_t thread_length = total_length / num_used_threads;
		const size_t thread_tail_length = total_length % num_used_threads;
		std::vector<std::future<void>> futures(num_used_threads);

		// launch the threads
        int next_idx = 0;

		#pragma omp unroll full
		for (size_t i = 0; i < thread_tail_length; i++)
		{
			futures[i] = pool.Submit(op_mat, &data[next_idx], &src.data[next_idx], &dst.data[next_idx], thread_length + 1);
			next_idx += thread_length + 1;
		}

		#pragma omp unroll full
		for (size_t i = thread_tail_length; i < num_used_threads; i++)
		{
			futures[i] = pool.Submit(op_mat, &data[next_idx], &src.data[next_idx], &dst.data[next_idx], thread_length);
			next_idx += thread_length;
		}
		// wait for the threads to finish
		#pragma omp unroll full
		for (size_t i = 0; i < num_used_threads; i++)
		{
			futures[i].wait();
		}

        return Status::SUCCESS;
    }

    /************************************************************************************************************************/
    // one input matrix, one output matrix
    template<auto op, typename... Args>
    inline Status Matrix::MatrixProc_ElemWise(Matrix& dst, ThreadPool& pool, Args... args) const
    {
        if (pool.used_threads <= 0)
		{
			return Status::INVALID_UTILIZATION;
		}

		if (this->Columns() != dst.Columns() || this->Rows() != dst.Rows())
		{
			return Status::INVALID_DIMENSIONS;
		}

		const size_t total_length = rows * columns;
        const size_t num_used_threads = std::min(pool.used_threads, total_length);

        auto op_mat = [&](const float* const __restrict  src, float* const __restrict  dst, const size_t size)
		{
            #pragma loop( ivdep )
            #pragma omp simd
			for (size_t i = 0; i < size; i++)
			{
				dst[i] = op(src[i],  args...);
			}
		};

        // divide the matrix among the threads
		const size_t thread_length = total_length / num_used_threads;
		const size_t thread_tail_length = total_length % num_used_threads;
		std::vector<std::future<void>> futures(num_used_threads);

		// launch the threads
        int next_idx = 0;

		#pragma omp unroll full
		for (size_t i = 0; i < thread_tail_length; i++)
		{
			futures[i] = pool.Submit(op_mat, &data[next_idx], &dst.data[next_idx], thread_length + 1);
			next_idx += thread_length + 1;
		}

		#pragma omp unroll full
		for (size_t i = thread_tail_length; i < num_used_threads; i++)
		{
			futures[i] = pool.Submit(op_mat, &data[next_idx], &dst.data[next_idx], thread_length);
			next_idx += thread_length;
		}
		// wait for the threads to finish
		#pragma omp unroll full
		for (size_t i = 0; i < num_used_threads; i++)
		{
			futures[i].wait();
		}

        return Status::SUCCESS;
    }
}