#pragma once

#include "matrix.h"
#include "vector.h"
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

	/************************************************************************************************************************/
    // matrix - vector operation
	template<auto op, typename... Args>
	Status Matrix::MatrixProc_ElemWise(const Vector& src, Matrix& dst, const BroadCast& broad_cast, ThreadPool& pool, Args... args) const
	{
		if (pool.used_threads <= 0)
		{
			return Status::INVALID_UTILIZATION;
		}

		if (this->Rows() != dst.Rows() || this->Columns() != dst.Columns())
		{
			return Status::INVALID_DIMENSIONS;
		}

		size_t num_used_threads = pool.used_threads;
		int dim_1, dim_2;
		std::function<void(const int, const int, const float* const, const float* const, const int, const int, float* const)> vec_mat;

		if (this->Rows() == src.Length() && broad_cast == BroadCast::BROAD_CAST_ROW)
		{
			dim_1 = columns;
			dim_2 = rows;

			vec_mat = [&](const int num_dim_1, const int start_dim_1, const float* const __restrict  p_src1,
				const float* const __restrict  p_src2, const int dim_1, const int dim_2,
				float* __restrict  const p_dst)
			{
				for (int d1 = start_dim_1; d1 < num_dim_1 + start_dim_1; d1++)
				{
					for (int d2 = 0; d2 < dim_2; d2++)
					{
						p_dst[d2 * dim_1 + d1] = op(p_src1[d2 * dim_1 + d1], p_src2[d2], args...);
					}
				}
			};
		}
		else if (this->Columns() == src.Length() && broad_cast == BroadCast::BROAD_CAST_COLUMN)
		{
			dim_1 = rows;
			dim_2 = columns;

			vec_mat = [&](const int num_dim_1, const int start_dim_1, const float* const __restrict  p_src1,
				const float* const __restrict  p_src2, const int dim_1, const int dim_2,
				float* __restrict  const p_dst)
			{
				for (int d1 = start_dim_1; d1 < num_dim_1 + start_dim_1; d1++)
				{
					for (int d2 = 0; d2 < dim_2; d2++)
					{
						p_dst[d1 * dim_2 + d2] = op(p_src1[d1 * dim_2 + d2], p_src2[d2], args...);
					}
				}
			};
		}
		else
		{
			return Status::INVALID_DIMENSIONS;
		}

		num_used_threads = dim_1 < num_used_threads ? dim_1 : num_used_threads;


		// divide the matrix among the threads
		const size_t dim_1_per_thread = dim_1 / num_used_threads;
		const size_t thread_tail = dim_1 % num_used_threads;
		std::vector<std::future<void>> futures(num_used_threads);
		// launch the threads
		int next_dim_1 = 0;

#pragma omp unroll full
		for (size_t i = 0; i < thread_tail; i++)
		{
			futures[i] = pool.Submit(vec_mat, dim_1_per_thread + 1, next_dim_1, data, src.data, dim_1, dim_2, dst.data);
			next_dim_1 += dim_1_per_thread + 1;
		}

#pragma omp unroll full
		for (size_t i = thread_tail; i < num_used_threads; i++)
		{
			futures[i] = pool.Submit(vec_mat, dim_1_per_thread, next_dim_1, data, src.data, dim_1, dim_2, dst.data);
			next_dim_1 += dim_1_per_thread;
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