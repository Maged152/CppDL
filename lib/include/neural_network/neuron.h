#pragma once

#include "vector.h"
#include <utility>
#include <functional>

namespace qlm
{
    class Neuron
    {
        private:
        Vector weights;
        float bias;

        std::function<float(const float)> activation_function;

        public:
        Neuron(const size_t length, std::function<float(const float)> af, const float min_value = 0.0f, const float max_value = 1.0f);

        public:
        std::pair<Status, float> Forward(const Vector& input, ThreadPool& pool);

        void Print() const;
    };

}