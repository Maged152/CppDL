#include "matrix.h"
#include "config.h"
#include <thread>
#include <vector>

namespace qlm
{
	Status Matrix::MatrixAdd(const Matrix& src, Matrix& dst, float utilization)
	{
		if (utilization <= 0)
		{
			return Status::INVALID_UTILIZATION;
		}

		if (src.Coulmns() != dst.Coulmns() || src.Rows() != dst.Rows() || src.Coulmns() != this->Coulmns() || src.Rows() != this->Rows())
		{
			return Status::INVALID_DIMENTIONS;
		}

		const unsigned int num_used_threads = utilization > 1.0f ? static_cast<unsigned int>(utilization) : static_cast<unsigned int>(utilization * num_threads);
		const unsigned int total_length = rows * columns;

		auto add_mat = [](const float* __restrict  src1, const float* __restrict  src2, float* __restrict  dst, const unsigned int size)
		{
			// vectorize
			for (int i = 0; i < size; i++)
			{
				dst[i] = src1[i] + src2[i];
			}
		};
		// divide the matrix among the threads
		const unsigned int thread_length = total_length / num_used_threads;
		const unsigned int thread_tail_length = total_length % num_used_threads;
		const unsigned int first_thread_length = thread_length + thread_tail_length;
		std::vector<std::thread> threads(num_used_threads);
		// launch the threads
		threads[0] = std::thread(add_mat, data, src.data, dst.data, first_thread_length);
		int next_idx = first_thread_length;
		for (unsigned int i = 1; i < num_used_threads; i++)
		{
			threads[i] = std::thread(add_mat, &data[next_idx], &src.data[next_idx], &dst.data[next_idx], thread_length);
			next_idx += thread_length;
		}
		// wait for the threads to finish
		for (unsigned int i = 0; i < num_used_threads; i++)
		{
			threads[i].join();
		}

		return Status::SUCCESS;
	}
}