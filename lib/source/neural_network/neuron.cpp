#include "neural_network/neuron.h"
#include "matrix.h"
#include <iostream>

namespace qlm
{ 
    Neuron::Neuron(const size_t length, std::function<float(const float)> af, const float min_value, const float max_value) : bias(0), weights(length), activation_function(af)
    {
        weights.RandomInit(min_value, max_value);
    }

    std::pair<Status, float> Neuron::Forward(const Vector& input, ThreadPool& pool)
    {
        float output {0};
        auto status = input.WeightedSum(weights, bias, output, pool);
        output = activation_function(output);
        return std::make_pair(status, output);
    }

    Status Neuron::Forward(const Matrix& input, Vector& output, ThreadPool& pool)
    {
        if (pool.used_threads <= 0)
		{
			return Status::INVALID_UTILIZATION;
		}

		if (output.Length() != input.Rows() || input.Columns() != weights.Length())
		{
			return Status::INVALID_DIMENSIONS;
		}

		size_t num_used_threads = pool.used_threads;
		const size_t total_rows = input.Rows();
		num_used_threads = total_rows < num_used_threads ? total_rows : num_used_threads;

		auto forward_mat = [&](const int num_rows, const int start_r, const float* const __restrict  p_src1, 
			                   const float* const __restrict  p_src2, const int in_c,
			                   float* __restrict  const p_dst)
		{
			for (int r = start_r; r < num_rows + start_r; r++)
			{
				float sum = 0;
                #pragma omp simd reduction(+:sum)
				for (int c = 0; c < in_c; c++)
				{
					sum += p_src1[r * in_c + c] * p_src2[c];	
				}
				p_dst[r] = activation_function(sum + bias);
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
			futures[i] = pool.Submit(forward_mat, rows_per_thread + 1, next_row, input.data, weights.data, input.columns, output.data);
			next_row += rows_per_thread + 1;
		}

		#pragma omp unroll full
		for (size_t i = thread_tail; i < num_used_threads; i++)
		{
			futures[i] = pool.Submit(forward_mat, rows_per_thread, next_row, input.data, weights.data, input.columns, output.data);
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

    void Neuron::Print() const
    {
        std::cout << "weights = [";
        weights.Print();
        std::cout << "]\n";
        std::cout << "bias = " << bias << "\n";
    }

}