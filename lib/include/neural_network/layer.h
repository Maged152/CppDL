#pragma once

#include "neuron.h"
#include <vector>

namespace qlm
{
    class Layer
    {
        public:
        virtual Status Forward(const Vector& input, ThreadPool& pool) = 0;
    };

    class Dense : public Layer
    {
        private:
        std::vector<Neuron> neurons;
        Vector output;
        size_t input_len;

        public:
        Dense(const size_t input_len, const size_t num_neurons, std::function<float(const float)> af, const float min_value = 0.0f, const float max_value = 1.0f);

        // Forward pass through the layer
        Status Forward(const Vector& input, ThreadPool& pool);

        Vector& Output();

        void Print() const;
    };
}