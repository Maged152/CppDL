#include "neural_network/layer.h"

namespace qlm
{
    template<auto AF>
    Dense<AF>::Dense(const size_t input_len, const size_t num_neurons, const float min_value, const float max_value) 
            : neurons(num_neurons, Neuron<AF>(input_len, min_value, max_value)), output(num_neurons), input_len(input_len) {}

    template<auto AF>
    Status Dense<AF>::Forward(const std::vector<float>& input, ThreadPool& pool)
    {
        // check input length
        
        // divide threads between neurons

        // launch neurons

        // get outputs
    }
}