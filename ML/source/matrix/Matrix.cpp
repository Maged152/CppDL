#include "matrix.h"
#include <random>
#include <iostream>
#include <iomanip>
#include <functional>

namespace qlm
{
	// matrix element wise operations +,-,*,/
	Status Matrix::Add(const Matrix& src, Matrix& dst, ThreadPool& pool)
	{
		return this->MatrixElemWiseOp<std::plus<float>>(src, dst, pool);
	}

	Status Matrix::Sub(const Matrix& src, Matrix& dst, ThreadPool& pool)
	{
		return this->MatrixElemWiseOp<std::minus<float>>(src, dst, pool);
	}

	Status Matrix::Mul(const Matrix& src, Matrix& dst, ThreadPool& pool)
	{
		return this->MatrixElemWiseOp<std::multiplies<float>>(src, dst, pool);
	}

	Status Matrix::Div(const Matrix& src, Matrix& dst, ThreadPool& pool)
	{
		return this->MatrixElemWiseOp<std::divides<float>>(src, dst, pool);
	}

	// matrix scalar wise operations +,-,*,/
	Status Matrix::Add(const float src, Matrix& dst, ThreadPool& pool)
	{
		return this->MatrixElemOp<std::plus<float>>(src, dst, pool);
	}

	Status Matrix::Sub(const float src, Matrix& dst, ThreadPool& pool)
	{
		return this->MatrixElemOp<std::minus<float>>(src, dst, pool);
	}

	Status Matrix::Mul(const float src, Matrix& dst, ThreadPool& pool)
	{
		return this->MatrixElemOp<std::multiplies<float>>(src, dst, pool);
	}

	Status Matrix::Div(const float src, Matrix& dst, ThreadPool& pool)
	{
		return this->MatrixElemOp<std::divides<float>>(src, dst, pool);
	}

	// matrix vector  operations 
	Status Matrix::Add(const Vector& src, Matrix& dst, const BroadCast& broad_cast, ThreadPool& pool)
	{
		return this->MatrixVectorOp<std::plus<float>>(src, dst, broad_cast, pool);
	}

	Status Matrix::Sub(const Vector& src, Matrix& dst, const BroadCast& broad_cast, ThreadPool& pool)
	{
		return this->MatrixVectorOp<std::minus<float>>(src, dst, broad_cast, pool);
	}

	Status Matrix::Mul(const Vector& src, Matrix& dst, const BroadCast& broad_cast, ThreadPool& pool)
	{
		return this->MatrixVectorOp<std::multiplies<float>>(src, dst, broad_cast, pool);
	}

	Status Matrix::Div(const Vector& src, Matrix& dst, const BroadCast& broad_cast, ThreadPool& pool)
	{
		return this->MatrixVectorOp<std::divides<float>>(src, dst, broad_cast, pool);
	}

	// general matrix functions
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