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

		if (this->Coulmns() != src.Rows() || this->Rows() != dst.Rows() || src.Coulmns() != dst.Coulmns())
		{
			return Status::INVALID_DIMENTIONS;
		}

		unsigned int num_used_threads = utilization > 1.0f ? static_cast<unsigned int>(utilization) : static_cast<unsigned int>(utilization * num_threads);
		const unsigned int total_rows = rows;
		num_used_threads = total_rows < num_used_threads ? total_rows : num_used_threads;

		auto dot_mat = [](const int num_rows, const float* const __restrict  p_row, 
			             const float* const __restrict  p_mat, const int in_r, const int in_c,
			             float* __restrict  p_dst)
		{
#pragma loop( ivdep )
#pragma omp simd
			for (int row = 0; row < num_rows; row++)
			{
				const int idx = row * in_r;
				float sum = 0;
				for (int col = 0; col < in_r; col++)
				{
					sum += p_row[idx + col] * p_mat[];
				}
			}
		};
		// divide the matrix among the threads
		const unsigned int rows_per_thread = total_rows / num_used_threads;
		const unsigned int thread_tail = total_rows % num_used_threads;
		const unsigned int first_thread_length = rows_per_thread + thread_tail ;
		std::vector<std::future<void>> futures(num_used_threads);
		// launch the threads
		futures[0] = std::async(std::launch::async, add_mat, data, src.data, dst.data, first_thread_length);
		int next_idx = first_thread_length;

		for (unsigned int i = 1; i < num_used_threads; i++)
		{
			futures[i] = std::async(std::launch::async, add_mat, &data[next_idx], &src.data[next_idx], &dst.data[next_idx], thread_length);
			next_idx += thread_length;
		}
		// wait for the threads to finish
		for (unsigned int i = 0; i < num_used_threads; i++)
		{
			futures[i].wait();
		}

		return Status::SUCCESS;
	}


}