#include "matrix.h"
#include <vector>
#include <functional>
#include <omp.h>
#include <future>

namespace qlm
{
	Status Matrix::Transpose(Matrix& dst, ThreadPool& pool) const 
	{
		if (pool.used_threads <= 0)
		{
			return Status::INVALID_UTILIZATION;
		}

		if (this->Columns() != dst.Rows() || this->Rows() != dst.Columns())
		{
			return Status::INVALID_DIMENSIONS;
		}

		size_t num_used_threads = pool.used_threads;
		int dim_1, dim_2;
		std::function<void(const int, const int, const float* const, const int, const int, float* const)> transpose_mat;

		if (rows > columns)
		{
			dim_1 = rows;
			dim_2 = columns;

			transpose_mat = [](const int num_dim_1, const int start_dim_1, const float* const __restrict  p_src,
				const int in_dim_1, const int in_dim_2,
				float* __restrict  const p_dst)
			{
				for (int r = start_dim_1; r < num_dim_1 + start_dim_1; r++)
				{
					for (int c = 0; c < in_dim_2; c++)
					{
						p_dst[c * in_dim_1 + r] = p_src[r * in_dim_2 + c];
					}
				}
			};
		}
		else
		{
			dim_1 = columns;
			dim_2 = rows;

			transpose_mat = [](const int num_dim_1, const int start_dim_1, const float* const __restrict  p_src,
				const int in_dim_1, const int in_dim_2,
				float* __restrict  const p_dst)
			{
				for (int r = start_dim_1; r < num_dim_1 + start_dim_1; r++)
				{
					for (int c = 0; c < in_dim_2; c++)
					{
						p_dst[r * in_dim_2 + c] = p_src[c * in_dim_1 + r];
					}
				}
			};
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
			futures[i] = pool.Submit(transpose_mat, dim_1_per_thread + 1, next_dim_1, data, dim_1, dim_2, dst.data);
			next_dim_1 += dim_1_per_thread + 1;
		}

		#pragma omp unroll full
		for (size_t i = thread_tail; i < num_used_threads; i++)
		{
			futures[i] = pool.Submit(transpose_mat, dim_1_per_thread, next_dim_1, data, dim_1, dim_2, dst.data);
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