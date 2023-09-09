#include "../../include/Test_Vector.h"
#include "../../include/test_utils.h"
#include "shakhbat_ml.h"
#include <iostream>
#include <windows.h>
#include <numbers>

using namespace test;
using namespace qlm;
using namespace std;

void TestVectorAngle(const Vector& src1, const Vector& src2, float& angle)
{
	// mag for src1
	float mag1 = 0;
	for (int l = 0; l < src1.Length(); l++)
	{
		mag1 += src1.Get(l) * src1.Get(l);
	}
	mag1 = std::sqrtf(mag1);
	// mag for src2
	float mag2 = 0;
	for (int l = 0; l < src2.Length(); l++)
	{
		mag2 += src2.Get(l) * src2.Get(l);
	}
	mag2 = std::sqrtf(mag2);
	// dot product
	float dot = 0;
	for (int l = 0; l < src1.Length(); l++)
	{
		dot += src1.Get(l) * src2.Get(l);
	}

	angle = std::acosf(dot / (mag1 * mag2)) * 180.0f / std::numbers::pi;
}

void test::Test_VectorAngle(std::vector<int>& vec_len, float utilization, float threshold, float min, float max)
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

		Vector src1{ len };
		Vector src2{ len };

		float dst_ref = 0;
		float dst_opt = 0;
		// random initialization
		src2.RandomInit(min, max);
		src2.RandomInit(min, max);
		// test code
		ref.Start();
		TestVectorAngle(src1, src2, dst_ref);
		ref.End();
		// multi-threading code
		opt.Start();
		auto status = src1.Angle(src2, dst_opt, utilization);
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