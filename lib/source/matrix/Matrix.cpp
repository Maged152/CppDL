#include "matrix.h"
#include <random>
#include <iostream>
#include <iomanip>
#include <functional>
#include "lambda.h"
#include "common/MatrixPcoc.h"
namespace qlm
{
	// Default constructor
	Matrix::Matrix() : data(nullptr), columns(0), rows(0)
	{}

	// Parameterized constructor
	Matrix::Matrix(int r, int c) : columns(c), rows(r) 
	{
		data = new float[columns * rows];
	}

	// Copy constructor
	Matrix::Matrix(const Matrix& other) : columns(other.columns), rows(other.rows) 
	{
		data = new float[columns * rows];
		for (int i = 0; i < columns * rows; ++i) {
			data[i] = other.data[i];
		}
	}

	// Destructor
	Matrix::~Matrix() 
	{
		rows = columns = 0;
		if (data != nullptr)
			delete[] data;
	}

	// Setter for individual element
	void Matrix::Set(int row, int col, float value) 
	{
		if (row >= 0 && row < rows && col >= 0 && col < columns)
		{
			data[row * columns + col] = value;
		}
	}

	void Matrix::Set(int i, float value) {
		if (i >= 0 && i < columns * rows)
		{
			data[i] = value;
		}
	}

	// Getter for individual element
	float Matrix::Get(int row, int col) const 
	{
		if (row >= 0 && row < rows && col >= 0 && col < columns)
		{
			return data[row * columns + col];
		}
		return std::numeric_limits<float>::signaling_NaN();
	}

	float Matrix::Get(int i) const
	{
		if (i >= 0 && i < columns * rows)
		{
			return data[i];
		}
		return std::numeric_limits<float>::signaling_NaN();
	}

	// Getter for columns
	int Matrix::Columns() const
	{
		return columns;
	}

	// Getter for rows
	int Matrix::Rows() const
	{
		return rows;
	}

	// matrix element wise operations
	Status Matrix::Add(const Matrix& src, Matrix& dst, ThreadPool& pool)
	{
		return this->MatrixProc_ElemWise<plus>(src, dst, pool);
	}

	Status Matrix::Sub(const Matrix& src, Matrix& dst, ThreadPool& pool)
	{
		return this->MatrixProc_ElemWise<minus>(src, dst, pool);
	}

	Status Matrix::Mul(const Matrix& src, Matrix& dst, ThreadPool& pool)
	{
		return this->MatrixProc_ElemWise<multiplies>(src, dst, pool);
	}

	Status Matrix::Div(const Matrix& src, Matrix& dst, ThreadPool& pool)
	{
		return this->MatrixProc_ElemWise<divides>(src, dst, pool);
	}

	// matrix scalar wise operations
	Status Matrix::Add(const float src, Matrix& dst, ThreadPool& pool)
	{
		return this->MatrixProc_ElemWise<plus>(dst, pool, src);
	}

	Status Matrix::Sub(const float src, Matrix& dst, ThreadPool& pool)
	{
		return this->MatrixProc_ElemWise<minus>(dst, pool, src);
	}

	Status Matrix::Mul(const float src, Matrix& dst, ThreadPool& pool)
	{
		return this->MatrixProc_ElemWise<multiplies>(dst, pool, src);
	}

	Status Matrix::Div(const float src, Matrix& dst, ThreadPool& pool)
	{
		return this->MatrixProc_ElemWise<divides>(dst, pool, src);
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