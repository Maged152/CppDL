#pragma once

#include <cmath>
#include "vector.h"

namespace qlm
{
	// The Step Activation Function
	inline float Step_AF(const float input)
	{
		return input > 0.0f ? 1.0f : 0.0f;
	}

	// The Linear Activation Function
	inline float Linear_AF(const float input)
	{
		return input;
	}

	// The Sigmoid Activation Function
	inline float Sigmoid_AF(const float input)
	{
		return 1.0f / (1.0f + std::exp(-input));
	}

	// The Rectified Linear Activation Function
	inline float ReLU_AF(const float input)
	{
		return input > 0.0f ? input : 0.0f;
	}

	// The Hyperbolic Tangent Activation Function
	inline float Tanh_AF(const float input)
	{
		return 2.0f * Sigmoid_AF(2.0f * input) - 1;
	}

	// The Leaky ReLU Activation Function
	inline float LeakyReLU_AF(const float input)
	{
		return input > 0.0f ? input : 0.01f * input;
	}

	// The Swish Activation Function
	inline float Swish_AF(const float input)
	{
		return input * Sigmoid_AF(input);
	}

	// The Soft Max Activation Function
	inline void SoftMax_AF(const Vector& input, Vector& output, ThreadPool& pool)
	{
		auto exp = [](const float src)
		{
			return  std::exp(src);
		};

		input.VectorProc_ElemWise<exp>(output, pool);
		float sum = 0.0f;
		output.Sum(sum, pool);
		output.Div(sum, output, pool);
	}
}