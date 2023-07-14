#include "matrix.h"
#include "config.h"
#include <vector>
#include <omp.h>
#include <future>

namespace qlm
{
	template<typename op>
	inline Status Matrix::MatrixElemWiseOp(const Matrix& src, Matrix& dst, float utilization)
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
			#pragma loop( ivdep )
			#pragma omp simd
			for (unsigned int i = 0; i < size; i++)
			{
				dst[i] = op()(src1[i], src2[i]);
			}
		};
		// divide the matrix among the threads
		const unsigned int thread_length = total_length / num_used_threads;
		const unsigned int thread_tail_length = total_length % num_used_threads;
		const unsigned int first_thread_length = thread_length + thread_tail_length;
		//std::vector<std::thread> threads(num_used_threads);
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


	template Status Matrix::MatrixElemWiseOp<std::plus<float>>(const Matrix&, Matrix&, float );
	template Status Matrix::MatrixElemWiseOp<std::minus<float>>(const Matrix&, Matrix&, float);
	template Status Matrix::MatrixElemWiseOp<std::multiplies<float>>(const Matrix&, Matrix&, float);
	template Status Matrix::MatrixElemWiseOp<std::divides<float>>(const Matrix&, Matrix&, float);
}