#pragma once

#include "neuron.h"
#include <vector>

namespace qlm
{
    template<auto AF>
    class Layer
    {
        public:
        Status Forward(ThreadPool& pool) = 0;
    };

    template<auto AF>
    class Dense : public Layer<AF>
    {
        private:
        std::vector<Neuron<AF>> neurons;
        Vector output;
        size_t input_len;

        public:
        Dense(const size_t input_len, const size_t num_neurons, const float min_value = 0.0f, const float max_value = 1.0f);

        // Forward pass through the layer
        Status Forward(const std::vector<float>& input, ThreadPool& pool);
        // {
        //     std::vector<float> outputs;
        //     outputs.reserve(neurons.size());

        //     if (input.size() != neurons[0].GetInputSize()) {
        //         // Handle input size mismatch error
        //         return {Status::Error, outputs};
        //     }

        //     for (const auto& neuron : neurons) {
        //         auto [status, output] = neuron.Forward(input, pool);
        //         if (status != Status::OK) {
        //             // Handle error
        //             return {status, outputs};
        //         }
        //         outputs.push_back(output);
        //     }

        //     return {Status::OK, outputs};
        // }
    };
}