#include "neural_network/neuron.h"
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

    void Neuron::Print() const
    {
        std::cout << "weights = [";
        weights.Print();
        std::cout << "]\n";
        std::cout << "bias = " << bias << "\n";
    }

}