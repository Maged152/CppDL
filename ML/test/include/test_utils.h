#pragma once
#include "shakhbat_ml.h"
#include <cmath>

#define CONSOLE_COLOR_BLACK 0
#define CONSOLE_COLOR_BLUE 1
#define CONSOLE_COLOR_GREEN 2
#define CONSOLE_COLOR_CYAN 3
#define CONSOLE_COLOR_RED 4
#define CONSOLE_COLOR_MAGENTA 5
#define CONSOLE_COLOR_YELLOW 6
#define CONSOLE_COLOR_WHITE 7
#define CONSOLE_COLOR_GRAY 8
#define CONSOLE_COLOR_LIGHT_BLUE 9
#define CONSOLE_COLOR_LIGHT_GREEN 10
#define CONSOLE_COLOR_LIGHT_CYAN 11
#define CONSOLE_COLOR_LIGHT_RED 12
#define CONSOLE_COLOR_LIGHT_MAGENTA 13
#define CONSOLE_COLOR_LIGHT_YELLOW 14
#define CONSOLE_COLOR_BRIGHT_WHITE 15


namespace test
{
	// compare
	inline bool TestCompare(const qlm::Matrix& mat1, const qlm::Matrix& mat2, const float threshold)
	{
		for (int i = 0; i < mat1.Rows() * mat1.Coulmns(); i++)
		{
			if (std::abs(mat1.Get(i) - mat2.Get(i)) > threshold)
			{
				return false;
			}
		}

		return true;
	}
}