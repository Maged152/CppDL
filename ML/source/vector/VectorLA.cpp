#include "vector.h"
#include <numbers>

namespace qlm
{
	// magnitude
	Status Vector::Mag(float& dst, ThreadPool& pool) const
	{
		const auto status = this->Dot(*this, dst, pool);
		dst = std::sqrt(dst);
		return status;
	}

	// unit vector
	Status Vector::Unit(Vector& dst, ThreadPool& pool) const
	{
		// get magnitude
		float mag{ 0 };
		auto status = this->Mag(mag, pool);
		// check status
		if (status != Status::SUCCESS)  return status;
	
		// unit = vector / magnitude
		status = this->Div(mag, dst, pool);

		return status;
	}

	// angle
	Status Vector::Angle(const Vector& src, float& dst, ThreadPool& pool) const
	{
		const int old_pool_size = pool.used_threads;
		const int new_pool_size = old_pool_size / 3;
		pool.used_threads = new_pool_size;

		float dot{0}, mag1{0}, mag2{0};

		auto bind_dot  = [&]() { return this->Dot(src, dot, pool); };
		auto bind_mag1 = [&]() { return this->Mag(mag1, pool); };
		auto bind_mag2 = [&]() { return src.Mag(mag2, pool); };

		auto fut_dot = pool.Submit(bind_dot);
		auto fut_mag1 = pool.Submit(bind_mag1);
		auto fut_mag2 = pool.Submit(bind_mag2);

		// dot product
		auto status = fut_dot.get();
		if (status != Status::SUCCESS)
		{
			return status;
			pool.used_threads = old_pool_size;
		}

		// magnitude for first vector
		status = fut_mag1.get();
		if (status != Status::SUCCESS)
		{
			return status;
			pool.used_threads = old_pool_size;
		}
		// magnitude for second vector
		status = fut_mag2.get();

		dst = std::acos(dot / (mag1 * mag2)) * 180.0f / std::numbers::pi;

		pool.used_threads = old_pool_size;

		return status;
	}

	//
	Status qlm::Vector::Corr(const Vector& src, float& dst, ThreadPool& pool) const
	{
		const int old_pool_size = pool.used_threads;
		const int new_pool_size = old_pool_size / 3;
		pool.used_threads = new_pool_size;

		float cov, var1, var2;

		auto bind_cov = [&]() { return this->Cov(src, cov, pool); };
		auto bind_var1 = [&]() { return this->Var(var1, pool); };
		auto bind_var2 = [&]() { return src.Var(var2, pool); };

		auto fut_cov = pool.Submit(bind_cov);
		auto fut_var1 = pool.Submit(bind_var1);
		auto fut_var2 = pool.Submit(bind_var2);

		auto status = fut_cov.get();
		if (status != Status::SUCCESS)
		{
			return status;
			pool.used_threads = old_pool_size;
		}

		status = fut_var1.get();
		if (status != Status::SUCCESS)
		{
			return status;
			pool.used_threads = old_pool_size;
		}
		
		status = fut_var2.get();
		if (status != Status::SUCCESS)
		{
			return status;
			pool.used_threads = old_pool_size;
		}

		dst = cov / std::sqrt(var1 * var2);

		pool.used_threads = old_pool_size;

		return status;
	}
}