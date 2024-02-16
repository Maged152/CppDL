#pragma once

#include "matrix.h"
#include "vector.h"
#include <vector>
#include <omp.h>
#include <future>

namespace qlm
{
    template<auto op, typename... Args>
    Status Matrix::MatrixProc_Dot(const Matrix& src, Matrix& dst,ThreadPool& pool, Args... args) const
    {
        if (pool.used_threads <= 0)
		{
			return Status::INVALID_UTILIZATION;
		}

		if (this->Columns() != src.Rows() || this->Rows() != dst.Rows() || src.Columns() != dst.Columns())
		{
			return Status::INVALID_DIMENSIONS;
		}

		size_t num_used_threads = pool.used_threads;
		const size_t total_rows = rows;
		num_used_threads = total_rows < num_used_threads ? total_rows : num_used_threads;

		auto dot_mat = [&](const int num_rows, const int start_r, const float* const __restrict  p_src1, 
			             const float* const __restrict  p_src2, const int in_r, const int in_c,
			             float* __restrict  const p_dst)
		{
			for (int r = start_r; r < num_rows + start_r; r++)
			{
				for (int c = 0; c < in_c; c++)
				{
					float sum = 0;
					#pragma omp simd reduction(+:sum)
					for (int e = 0; e < in_r; e++)
					{
						sum = op(p_src1[r * in_r + e], p_src2[e * in_c + c], sum, args...);	
					}
					p_dst[r * in_c + c] =  sum;
				}
			}
		};
		// divide the matrix among the threads
		const size_t rows_per_thread = total_rows / num_used_threads;
		const size_t thread_tail = total_rows % num_used_threads;
		std::vector<std::future<void>> futures(num_used_threads);
		// launch the threads
		int next_row = 0;

		#pragma omp unroll full
		for (size_t i = 0; i < thread_tail; i++)
		{
			futures[i] = pool.Submit(dot_mat, rows_per_thread + 1, next_row, data, src.data, src.rows, src.columns, dst.data);
			next_row += rows_per_thread + 1;
		}

		#pragma omp unroll full
		for (size_t i = thread_tail; i < num_used_threads; i++)
		{
			futures[i] = pool.Submit(dot_mat, rows_per_thread, next_row, data, src.data, src.rows, src.columns, dst.data);
			next_row += rows_per_thread;
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