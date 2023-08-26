#include "matrix.h"
#include "vector.h"
#include "config.h"
#include <vector>
#include <functional>
#include <omp.h>
#include <future>

namespace qlm
{
	template<typename op>
	inline Status Matrix::MatrixVectorOp(const Vector& src, Matrix& dst, const BroadCast& broad_cast, float utilization)
	{
		if (utilization <= 0)
		{
			return Status::INVALID_UTILIZATION;
		}

		if (this->Rows() != dst.Rows() || this->Columns() != dst.Columns())
		{
			return Status::INVALID_DIMENTIONS;
		}

		unsigned int num_used_threads = utilization > 1.0f ? static_cast<unsigned int>(utilization) : static_cast<unsigned int>(utilization * num_threads);
		int dim_1, dim_2;
		std::function<void(const int, const int, const float* const, const float* const, const int, const int, float* const)> vec_mat;

		if (this->Rows() == src.Length() && broad_cast == BroadCast::BROAD_CAST_ROW)
		{
			dim_1 = columns;
			dim_2 = rows;

			vec_mat = [](const int num_dim_1, const int start_dim_1, const float* const __restrict  p_src1,
				const float* const __restrict  p_src2, const int dim_1, const int dim_2,
				float* __restrict  const p_dst)
			{
				for (int d1 = start_dim_1; d1 < num_dim_1 + start_dim_1; d1++)
				{
					for (int d2 = 0; d2 < dim_2; d2++)
					{
						p_dst[d2 * dim_1 + d1] = op()(p_src1[d2 * dim_1 + d1], p_src2[d2]);
					}
				}
			};
		}
		else if (this->Columns() == src.Length() && broad_cast == BroadCast::BROAD_CAST_COLUMN)
		{
			dim_1 = rows;
			dim_2 = columns;

			vec_mat = [](const int num_dim_1, const int start_dim_1, const float* const __restrict  p_src1,
				const float* const __restrict  p_src2, const int dim_1, const int dim_2,
				float* __restrict  const p_dst)
			{
				for (int d1 = start_dim_1; d1 < num_dim_1 + start_dim_1; d1++)
				{
					for (int d2 = 0; d2 < dim_2; d2++)
					{
						p_dst[d1 * dim_2 + d2] = op()(p_src1[d1 * dim_2 + d2], p_src2[d2]);
					}
				}
			};
		}
		else
		{
			return Status::INVALID_DIMENTIONS;
		}

		num_used_threads = dim_1 < num_used_threads ? dim_1 : num_used_threads;


		// divide the matrix among the threads
		const unsigned int dim_1_per_thread = dim_1 / num_used_threads;
		const unsigned int thread_tail = dim_1 % num_used_threads;
		std::vector<std::future<void>> futures(num_used_threads);
		// launch the threads
		int next_dim_1 = 0;

#pragma omp unroll full
		for (unsigned int i = 0; i < thread_tail; i++)
		{
			futures[i] = std::async(std::launch::async, vec_mat, dim_1_per_thread + 1, next_dim_1, data, src.data, dim_1, dim_2, dst.data);
			next_dim_1 += dim_1_per_thread + 1;
		}

#pragma omp unroll full
		for (unsigned int i = thread_tail; i < num_used_threads; i++)
		{
			futures[i] = std::async(std::launch::async, vec_mat, dim_1_per_thread, next_dim_1, data, src.data, dim_1, dim_2, dst.data);
			next_dim_1 += dim_1_per_thread;
		}

		// wait for the threads to finish
#pragma omp unroll full
		for (unsigned int i = 0; i < num_used_threads; i++)
		{
			futures[i].wait();
		}

		return Status::SUCCESS;
	}

	template Status Matrix::MatrixVectorOp<std::plus<float>>(const Vector&, Matrix&, const BroadCast&, float);
	template Status Matrix::MatrixVectorOp<std::minus<float>>(const Vector&, Matrix&, const BroadCast&, float);
	template Status Matrix::MatrixVectorOp<std::multiplies<float>>(const Vector&, Matrix&, const BroadCast&, float);
	template Status Matrix::MatrixVectorOp<std::divides<float>>(const Vector&, Matrix&, const BroadCast&, float);
}