#include "matrix.h"
#include <random>
#include <iostream>
#include <iomanip>
#include <functional>

namespace qlm
{
	// matrix element wise operations
	Status Matrix::MatrixAdd(const Matrix& src, Matrix& dst, float utilization)
	{
		return this->MatrixElemWiseOp<std::plus<float>>(src, dst, utilization);
	}

	void Matrix::RandomInit(const float min_value, const float max_value)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dis(min_value, max_value);

		for (int i = 0; i < columns * rows; i++)
		{
			this->Set(i, dis(gen));
		}
	}

	void Matrix::Print()
	{
		for (int r = 0; r < rows; r++)
		{
			int number_digits = 5;

			for (int c = 0; c < columns; c++)
			{
				float element = this->Get(r, c);

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