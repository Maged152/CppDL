#include "vector.h"
#include <random>
#include <iostream>
#include <iomanip>
#include <functional>
#include "common/VectorPcoc.h"
#include "lambda.h"


namespace qlm
{
	// Default constructor
	Vector::Vector() : data(nullptr), len(0)
	{}
	// Parameterized constructor
	Vector::Vector(int l) : len(l)
	{
		data = new float[l];
	}
	// Copy constructor
	Vector::Vector(const Vector& other) : len(other.len)
	{
		data = new float[len];
		for (int i = 0; i < len; ++i) {
			data[i] = other.data[i];
		}
	}
	// Destructor
	Vector::~Vector()
	{
		if (data != nullptr)
			delete[] data;
	}

	// Setter for individual element
	void Vector::Set(int i, float value) {
		if (i >= 0 && i < len)
		{
			data[i] = value;
		}
	}
	// Getter for individual element
	float Vector::Get(int i) const
	{
		if (i >= 0 && i <len)
		{
			return data[i];
		}
		return std::numeric_limits<float>::signaling_NaN();
	}
	// Getter for length
	int Vector::Length() const
	{
		return len;
	}
	// allocate memory
	void Vector::Alloc(const size_t l)
	{
		if (data != nullptr)
		{
			delete[] data;
		}

		data = new float[l];
		len = l;
	}

	// Vector element wise operations +,-,*,/
	Status Vector::Add(const Vector& src, Vector& dst, ThreadPool& pool) const
	{
		return this->VectorProc_ElemWise<plus>(src, dst, pool);
	}

	Status Vector::Sub(const Vector& src, Vector& dst, ThreadPool& pool) const
	{
		return this->VectorProc_ElemWise<minus>(src, dst, pool);
	}

	Status Vector::Mul(const Vector& src, Vector& dst, ThreadPool& pool) const
	{
		return this->VectorProc_ElemWise<multiplies>(src, dst, pool);
	}

	Status Vector::Div(const Vector& src, Vector& dst, ThreadPool& pool) const
	{
		return this->VectorProc_ElemWise<divides>(src, dst, pool);
	}

	// Vector scalar wise operations +,-,*,/
	Status Vector::Add(const float src, Vector& dst, ThreadPool& pool) const
	{
		return this->VectorProc_ElemWise<plus>(dst, pool, src);
	}

	Status Vector::Sub(const float src, Vector& dst, ThreadPool& pool) const
	{
		return this->VectorProc_ElemWise<minus>(dst, pool, src);
	}

	Status Vector::Mul(const float src, Vector& dst, ThreadPool& pool) const
	{
		return this->VectorProc_ElemWise<multiplies>(dst, pool, src);
	}

	Status Vector::Div(const float src, Vector& dst, ThreadPool& pool) const
	{
		return this->VectorProc_ElemWise<divides>(dst, pool, src);
	}
	// vector operations
	Status Vector::Mean(float& dst, ThreadPool& pool) const
	{
		const auto status = this->Sum(dst, pool);
		dst /= len;

		return status;
	}

	Status Vector::Min(float& dst, ThreadPool& pool) const
	{
		return this->VectorProc_1Scalar_Out<min>(dst, pool);
	}

	Status Vector::MinMax(float& dst_min, float& dst_max, ThreadPool& pool) const
	{
		return this->VectorProc_2Scalar_Out<min_max>(dst_min, dst_max, pool);
	}

	Status Vector::Max(float& dst, ThreadPool& pool) const
	{
		return this->VectorProc_1Scalar_Out<max>(dst, pool);
	}

	Status Vector::Norm(const qlm::Norm norm, float& dst, ThreadPool& pool) const
	{
		if (norm == qlm::Norm::L1_NORM)
		{
			return this->VectorProc_1Scalar_Out<l1_norm>(dst, pool);
		}
		else if (norm == qlm::Norm::L2_NORM)
		{
			return this->Mag(dst, pool);
		}
		else
		{
			return this->Max(dst, pool);
		}
		return Status();
	}

	Status Vector::Sum(float& dst, ThreadPool& pool) const
	{
		return this->VectorProc_1Scalar_Out<sum>(dst, pool);
	}

	Status Vector::Dot(const Vector& src, float& dst, ThreadPool& pool) const
	{
		return VectorProc_1Scalar_Out<dot>(src, dst, pool);
	}

	Status Vector::ArgMin(size_t& dst, ThreadPool& pool) const
	{
		return this->VectorProc_1ArgScalar_Out<arg_min>(dst, pool);
	}

	Status Vector::ArgMax(size_t& dst, ThreadPool& pool) const
	{
		return this->VectorProc_1ArgScalar_Out<arg_max>(dst, pool);
	}

	Status Vector::ArgMinMax(size_t& dst_min, size_t& dst_max, ThreadPool& pool) const
	{
		return VectorProc_2ArgScalar_Out<arg_min_max>(dst_min, dst_max, pool);
	}

	Status Vector::WeightedSum(const Vector& weights, const float bias, float& dst, ThreadPool& pool) const
	{
		auto status = Dot(weights, dst, pool);
		dst += bias;

		return status;
	}

	// Vector helper functions
	void Vector::RandomInit(const float min_value, const float max_value)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dis(min_value, max_value);

		for (int i = 0; i < len; i++)
		{
			this->Set(i, dis(gen));
		}
	}

	void Vector::Print() const
	{
		int number_digits = 5;

		for (int l = 0; l < len; l++)
		{
			float element = this->Get(l);

			if (element != 0)
			{
				int digits = static_cast<int>(std::log10(std::abs(element))) + 1;
				number_digits = digits >= 5 ? 0 : 5 - digits;
			}

			std::cout << std::fixed << std::setprecision(number_digits) << element << " ";
		}
	}
}