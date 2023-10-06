#include "../../include/Test_Vector.h"
#include "../../include/test_utils.h"
#include "shakhbat_ml.h"
#include <iostream>
#include <windows.h>

using namespace test;
using namespace qlm;
using namespace std;

void TestVectorDiv(const Vector& src1, const Vector& src2, Vector& dst)
{
	for (int l = 0; l < src1.Length(); l++)
	{
		float res = src1.Get(l) / src2.Get(l);
		dst.Set(l, res);
	}
}

void test::Test_VectorDiv(std::vector<int>& vec_len, int num_threads, float threshold, float min, float max)
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
	cout << "Vector div test\n";
	cout << "Number of test cases = " << vec_len.size() << "\n";

	int num_failed_cases = 0;

	// div two vector
	for (int l = 0; l < vec_len.size(); l++)
	{

		int len = vec_len[l];

		SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_GREEN);
		cout << "length  : " << len << "\n";

		ThreadPool pool{ num_threads };

		Timer<usec> ref;
		Timer<usec> opt;

		Vector src1{ len };
		Vector src2{ len };

		Vector dst_ref{ len };
		Vector dst_opt{ len };
		// random initialization
		src1.RandomInit(min, max);
		src2.RandomInit(min, max);
		// test Vector div
		ref.Start();
		TestVectorDiv(src1, src2, dst_ref);
		ref.End();
		// div Vector function
		opt.Start();
		auto status = src1.Div(src2, dst_opt, pool);
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
		std::cout << "All test casses PASSED\n";
	}

	SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_WHITE);
}