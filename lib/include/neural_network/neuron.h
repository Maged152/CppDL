#pragma once

#include "vector.h"
#include <utility>

namespace qlm
{
    template<auto AF>
    class Neuron
    {
        private:
        Vector weights;
        float bias;

        public:
        Neuron(const size_t length, const float min_value = 0.0f, const float max_value = 1.0f) : bias(0), weights(length)
        {
            weights.RandomInit(min_value, max_value);
        }

        public:
        std::pair<Status, float> Forward(const Vector& input, ThreadPool& pool)
        {
            float output {0};
            auto status = input.WeightedSum(weights, bias, output, pool);
            output = AF(output);
            return std::make_pair(status, output);
        }
    };

}