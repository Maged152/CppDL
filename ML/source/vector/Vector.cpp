#include "vector.h"
#include <random>
#include <iostream>
#include <iomanip>
#include <functional>
#include "common/VectorPcoc.h"


namespace qlm
{
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
		return this->VectorProc_ElemWise<plus>(src, dst, pool);
	}

	Status Vector::Sub(const float src, Vector& dst, ThreadPool& pool) const
	{
		return this->VectorProc_ElemWise<minus>(src, dst, pool);
	}

	Status Vector::Mul(const float src, Vector& dst, ThreadPool& pool) const
	{
		return this->VectorProc_ElemWise<multiplies>(src, dst, pool);
	}

	Status Vector::Div(const float src, Vector& dst, ThreadPool& pool) const
	{
		return this->VectorProc_ElemWise<divides>(src, dst, pool);
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

	Status Vector::ArgMin(unsigned int& dst, ThreadPool& pool) const
	{
		return this->VectorProc_1ArgScalar_Out<arg_min>(dst, pool);
	}

	Status Vector::ArgMax(unsigned int& dst, ThreadPool& pool) const
	{
		return this->VectorProc_1ArgScalar_Out<arg_max>(dst, pool);
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