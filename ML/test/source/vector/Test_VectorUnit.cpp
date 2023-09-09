#include "../../include/Test_Vector.h"
#include "../../include/test_utils.h"
#include "shakhbat_ml.h"
#include <iostream>
#include <windows.h>

using namespace test;
using namespace qlm;
using namespace std;

void TestVectorUnit(const Vector& src, Vector& dst)
{
	float mag = 0;
	for (int l = 0; l < src.Length(); l++)
	{
		mag += src.Get(l) * src.Get(l);
	}

	mag = std::sqrtf(mag);

	for (int l = 0; l < src.Length(); l++)
	{
		float res = src.Get(l) / mag;
		dst.Set(l, res);
	}
}

void test::Test_VectorUnit(std::vector<int>& vec_len, float utilization, float threshold, float min, float max)
{
	HANDLE col_handle;
	col_handle = GetStdHandle(STD_OUTPUT_HANDLE);

	if (vec_len.size() == 0)
	{
		SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_LIGHT_RED);
		cout << "Dimensions array must have values\n";
		return;
	}

	if (max <= min)
	{
		SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_LIGHT_RED);
		cout << "Max value must be greater than the min value\n";
		return;
	}

	SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_BLUE);
	cout << "Vector unit test\n";
	cout << "Number of test cases = " << vec_len.size() << "\n";

	int num_failed_cases = 0;


	for (int l = 0; l < vec_len.size(); l++)
	{

		int len = vec_len[l];

		SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_GREEN);
		cout << "length  : " << len << "\n";

		Timer<usec> ref;
		Timer<usec> opt;

		Vector src{ len };

		Vector dst_ref { len };
		Vector dst_opt { len };
		// random initialization
		src.RandomInit(min, max);
		// test code
		ref.Start();
		TestVectorUnit(src, dst_ref);
		ref.End();
		// multi-threading code
		opt.Start();
		auto status = src.Unit(dst_opt, utilization);
		opt.End();
		// compare the results
		bool res = TestCompare(dst_ref, dst_opt, threshold);

		PrintTestResults(res, status, ref, opt, col_handle);

		if (!res)
		{
			num_failed_cases++;
		}
	}

	if (num_failed_cases > 0)
	{
		SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_LIGHT_RED);
		std::cout << "Number of FAILED test cases  : " << num_failed_cases << "\n";
	}
	else
	{
		SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_GREEN);
		std::cout << "All test cases PASSED\n";
	}

	SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_WHITE);
}