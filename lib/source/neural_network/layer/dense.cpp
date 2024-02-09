#include "neural_network/layer.h"
#include <cmath>
#include <iostream>
namespace qlm
{
    
    Dense::Dense(const size_t input_len, const size_t num_neurons, std::function<float(const float)> af, const float min_value, const float max_value) 
    : output(num_neurons), input_len(input_len) 
    {
        for (size_t i = 0; i < num_neurons; ++i) 
        {
            neurons.push_back(Neuron(input_len, af, min_value, max_value));
        }
    }


    Status Dense::Forward(const Vector& input, ThreadPool& pool)
    {
        Status ret_status = Status::SUCCESS;

        // check input length
        if (input.Length() != input_len)
        {
            return Status::INVALID_DIMENSIONS;
        }

        // divide threads between neurons
        const size_t num_neurons = neurons.size();
        const int old_pool_size = pool.used_threads;
		const int new_pool_size = std::ceil(old_pool_size / num_neurons);
		pool.used_threads = new_pool_size;

        auto forward_neuron = [&](const int i) { return neurons[i].Forward(input, pool); };
        std::vector<std::future<std::pair<Status, float>>> futures(num_neurons);

        // launch neurons
        for (int i = 0; i < num_neurons; i++)
		{
			futures[i] = pool.Submit(forward_neuron, i);
		}

        // get outputs
        for (int i = 0; i < num_neurons; i++)
		{
			auto [status, output_neuron] = futures[i].get();
			
            if (status != Status::SUCCESS)
            {
                ret_status = status;
            }

            output.Set(i, output_neuron);
		}
        pool.used_threads = old_pool_size;

        return ret_status;
    }

    
    Vector& Dense::Output()
    {
        return output;
    }

    void Dense::Print() const
    {
        for (int i = 0; i < neurons.size(); i++)
		{
			std::cout << "neuron " << i << " :\n";
            neurons[i].Print();
		}
    }
}