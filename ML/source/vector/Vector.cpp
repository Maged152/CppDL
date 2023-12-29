#include "vector.h"
#include <random>
#include <iostream>
#include <iomanip>
#include <functional>
#include "common/VectorProc_1Scalar_Out.h"

namespace qlm
{
	// Vector element wise operations +,-,*,/
	Status Vector::Add(const Vector& src, Vector& dst, ThreadPool& pool) const
	{
		return this->VectorElemWiseOp<std::plus<float>>(src, dst, pool);
	}

	Status Vector::Sub(const Vector& src, Vector& dst, ThreadPool& pool) const
	{
		return this->VectorElemWiseOp<std::minus<float>>(src, dst, pool);
	}

	Status Vector::Mul(const Vector& src, Vector& dst, ThreadPool& pool) const
	{
		return this->VectorElemWiseOp<std::multiplies<float>>(src, dst, pool);
	}

	Status Vector::Div(const Vector& src, Vector& dst, ThreadPool& pool) const
	{
		return this->VectorElemWiseOp<std::divides<float>>(src, dst, pool);
	}

	// Vector scalar wise operations +,-,*,/
	Status Vector::Add(const float src, Vector& dst, ThreadPool& pool) const
	{
		return this->VectorElemOp<std::plus<float>>(src, dst, pool);
	}

	Status Vector::Sub(const float src, Vector& dst, ThreadPool& pool) const
	{
		return this->VectorElemOp<std::minus<float>>(src, dst, pool);
	}

	Status Vector::Mul(const float src, Vector& dst, ThreadPool& pool) const
	{
		return this->VectorElemOp<std::multiplies<float>>(src, dst, pool);
	}

	Status Vector::Div(const float src, Vector& dst, ThreadPool& pool) const
	{
		return this->VectorElemOp<std::divides<float>>(src, dst, pool);
	}
	// vector operations
	Status qlm::Vector::Mean(float& dst, ThreadPool& pool) const
	{
		const auto status = this->Sum(dst, pool);
		dst /= len;

		return status;
	}

	Status Vector::Min(float& dst, ThreadPool& pool) const
	{
		auto min_op = [](const float src, const float dst)
		{
			return src < dst ? src : dst;
		};

		return this->VectorProc_1Scalar_Out<min_op>(dst, pool);
	}

	Status Vector::Max(float& dst, ThreadPool& pool) const
	{
		auto max_op = [](const float src, const float dst)
		{
			return src > dst ? src : dst;
		};

		return this->VectorProc_1Scalar_Out<max_op>(dst, pool);
	}

	Status qlm::Vector::Norm(const NORM norm, float& dst, ThreadPool& pool) const
	{
		if (norm == qlm::NORM::L1_NORM)
		{
			auto l1_norm_lambda = [](const float dst, const float src)
			{
				return dst + std::abs(src);
			};

			return this->VectorProc_1Scalar_Out<l1_norm_lambda>(dst, pool);
		}
		else if (norm == qlm::NORM::L2_NORM)
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
		auto sum_op = [](const float src, const float dst)
		{
			return src + dst;
		};

		return this->VectorProc_1Scalar_Out<sum_op>(dst, pool);
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