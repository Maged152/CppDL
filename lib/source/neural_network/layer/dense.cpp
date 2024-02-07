#include "neural_network/layer.h"

namespace qlm
{
    template<auto AF>
    Dense<AF>::Dense(const size_t input_len, const size_t num_neurons, const float min_value, const float max_value) 
            : neurons(num_neurons, Neuron<AF>(input_len, min_value, max_value)), output(num_neurons), input_len(input_len) {}

    template<auto AF>
    Status Dense<AF>::Forward(const Vector& input, ThreadPool& pool)
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
                ret_status = status
            }

            output.Set(i, output_neuron);
		}
        pool.used_threads = old_pool_size;

        return ret_status;
    }

    template<auto AF>
    Vector& Dense<AF>::Output() const
    {
        return output;
    }
}