#include "matrix.h"
#include "config.h"
#include <cassert>

namespace qlm
{
	Status Matrix::MatrixAdd(const Matrix& src, Matrix& dst, float utilization)
	{
		if (utilization <= 0)
		{
			return Status::INVALID_UTILIZATION;
		}

		if (src.Height() != dst.Height() && src.Width() != dst.Width())
		{
			return Status::INVALID_DIMENTIONS;
		}

		const unsigned int num_used_threads = utilization > 1.0f ? static_cast<int>(utilization) : utilization * num_threads;


		return Status::SUCCESS;
	}
}