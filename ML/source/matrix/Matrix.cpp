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

	void Matrix::Print()
	{
		for (int i = 0; i < width; i++)
		{
			int number_digits = 5;

			for (int j = 0; j < height; j++)
			{
				float element = this->Get(i, j);

				if (element != 0)
				{
					int digits = static_cast<int>(std::log10(std::abs(element))) + 1;
					number_digits = digits >= 5 ? 0 : 5 - digits;
				}

				std::cout << std::fixed << std::setprecision(number_digits) << element << " ";
			}
			std::cout << std::endl;
		}
	}
}