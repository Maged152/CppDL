#pragma once

#include "vector.h"
#include "activation_functions.h"

namespace qlm
{
    template<auto AF>
    class Neuron
    {
        private:
        Vector weights;
        const Vector& input;
        float& output;
        float bias;

        public:
        Neuron(const Vector& input, float& output, const float min_value = 0.0f, const float max_value = 1.0f) : input(input), output(output), bias(0), weights(input.Length())
        {
            weights.RandomInit(min_value, max_value);
        }

        public:
        Status Forward(ThreadPool& pool)
        {
            auto status = input.WeightedSum(weights, bias, output, pool);
            output = AF(output);
            return status;
        }
    };

}