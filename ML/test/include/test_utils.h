#pragma once
#include "shakhbat_ml.h"
#include <cmath>
#include <iostream>
#include <random>
#include <windows.h>

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
		for (int i = 0; i < mat1.Rows() * mat1.Columns(); i++)
		{
			if (std::abs(mat1.Get(i) - mat2.Get(i)) > threshold)
			{
				return false;
			}
		}

		return true;
	}
	inline bool TestCompare(const qlm::Vector& vec1, const qlm::Vector& vec2, const float threshold)
	{
		for (int i = 0; i < vec1.Length(); i++)
		{
			if (std::abs(vec1.Get(i) - vec2.Get(i)) > threshold)
			{
				return false;
			}
		}

		return true;
	}

	inline bool TestCompare(const float& src1, const float& src2, const float threshold)
	{
		if (std::abs(src1 - src2) > threshold)
		{
			return false;
		}

		return true;
	}

	inline void PrintTestResults(bool res, qlm::Status status, const qlm::Timer<qlm::usec>& ref, const qlm::Timer<qlm::usec>& opt, const HANDLE& col_handle)
	{
		if (res && status == qlm::Status::SUCCESS)
		{
			// print output information
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_GREEN);
			std::cout << "opt code time  : " << opt.Duration() << " usec\n";
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_YELLOW);
			std::cout << "test code time  : " << ref.Duration() << " usec\n";

			if (opt.Duration() < ref.Duration())
			{
				SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_GREEN);
				std::cout << "opt code is faster by  : " << ((ref.Duration() - opt.Duration()) / opt.Duration()) * 100 << " %\n";
			}
			else
			{
				SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_LIGHT_YELLOW);
				std::cout << "opt code is slower by  : " << ((opt.Duration() - ref.Duration()) / ref.Duration()) * 100 << " %\n";
			}

			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_GREEN);
			std::cout << "STATUS  : " << "PASSED\n";
		}
		else
		{
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_LIGHT_RED);
			std::cout << "STATUS  : " << "FAILED\n";
		}
	}
}