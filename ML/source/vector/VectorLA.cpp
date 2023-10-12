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
		float dot{0}, mag1{0}, mag2{0};
		// dot product
		auto status = this->Dot(src, dot, pool);
		if (status != Status::SUCCESS)  return status;

		// magnitude for first vector
		status = this->Mag(mag1, pool);
		if (status != Status::SUCCESS)  return status;

		// magnitude for second vector
		status = src.Mag(mag2, pool);

		dst = std::acos(dot / (mag1 * mag2)) * 180.0f / std::numbers::pi;

		return status;
	}
}