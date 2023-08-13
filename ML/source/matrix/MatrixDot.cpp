#include "matrix.h"
#include "config.h"
#include <vector>
#include <omp.h>
#include <future>

namespace qlm
{
	Status Matrix::Dot(const Matrix& src, Matrix& dst, float utilization)
	{
		if (utilization <= 0)
		{
			return Status::INVALID_UTILIZATION;
		}

		if (this->Columns() != src.Rows() || this->Rows() != dst.Rows() || src.Columns() != dst.Columns())
		{
			return Status::INVALID_DIMENTIONS;
		}

		unsigned int num_used_threads = utilization > 1.0f ? static_cast<unsigned int>(utilization) : static_cast<unsigned int>(utilization * num_threads);
		const unsigned int total_rows = rows;
		num_used_threads = total_rows < num_used_threads ? total_rows : num_used_threads;

		auto dot_mat = [](const int num_rows, const int start_r, const float* const __restrict  p_src1, 
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
						sum += p_src1[r * in_r + e] * p_src2[e * in_c + c];
						
					}
					p_dst[r * in_c + c] =  sum;
				}
			}
		};
		// divide the matrix among the threads
		const unsigned int rows_per_thread = total_rows / num_used_threads;
		const unsigned int thread_tail = total_rows % num_used_threads;
		std::vector<std::future<void>> futures(num_used_threads);
		// launch the threads
		int next_row = 0;

		#pragma omp unroll full
		for (unsigned int i = 0; i < thread_tail; i++)
		{
			futures[i] = std::async(std::launch::async, dot_mat, rows_per_thread + 1, next_row, data, src.data, src.rows, src.columns, dst.data);
			next_row += rows_per_thread + 1;
		}

		#pragma omp unroll full
		for (unsigned int i = thread_tail; i < num_used_threads; i++)
		{
			futures[i] = std::async(std::launch::async, dot_mat, rows_per_thread, next_row, data, src.data, src.rows, src.columns, dst.data);
			next_row += rows_per_thread;
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