#include "matrix.h"
#include <random>
#include <iostream>
#include <iomanip>

namespace qlm
{
	void Matrix::RandomInit(const float min_value, const float max_value)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dis(min_value, max_value);

		for (int i = 0; i < height * width; i++)
		{
			this->Set(i, dis(gen));
		}
	}


}