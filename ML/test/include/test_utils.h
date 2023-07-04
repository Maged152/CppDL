#pragma once
#include "shakhbat_ml.h"
#include "../../include/console_color.h"
#include <cmath>
namespace test
{
	// compare
	bool TestCompare(const qlm::Matrix& mat1, const qlm::Matrix& mat2, const float threshold)
	{
		for (int i = 0; i < mat1.Height() * mat1.Width(); i++)
		{
			if (std::abs(mat1.Get(i) - mat2.Get(i)) > threshold)
			{
				return false;
			}
		}

		return true;
	}
}