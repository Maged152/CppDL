
#include "../../test.h"
#include "shakhbat_ml.h"
#include <iostream>
#include <windows.h>

using namespace test;
using namespace qlm;
using namespace std;


bool test::Test_VectorMinMax(std::vector<int>& vec_len, unsigned int  num_threads, float threshold, float min, float max)
{
	HANDLE col_handle;
	col_handle = GetStdHandle(STD_OUTPUT_HANDLE);

	if (vec_len.size() == 0)
	{
		SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_LIGHT_RED);
		cout << "Dimensions array must have values\n";
		return false;
	}

	if (max <= min)
	{
		SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_LIGHT_RED);
		cout << "Max value must be greater than the min value\n";
		return false;
	}

	SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_BLUE);
	cout << "Vector min-max test\n";
	cout << "Number of test cases = " << vec_len.size() << "\n";

	int num_failed_cases = 0;
	bool res = true;


	for (int l = 0; l < vec_len.size(); l++)
	{

		int len = vec_len[l];


		SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_GREEN);
		cout << "length  : " << len << "\n";

		Timer<usec> ref;
		Timer<usec> opt;

		ThreadPool pool{ num_threads };

		Vector src{ len };

		float dst_ref_min = 0;
		float dst_ref_max = 0;

		float dst_opt_min = 0;
		float dst_opt_max = 0;
		// random initialization
		src.RandomInit(min, max);
		// test code
		ref.Start();
		TestVectorMinMax(src, dst_ref_min, dst_ref_max);
		ref.End();
		// multi-threading code
		opt.Start();
		auto status = src.MinMax(dst_opt_min, dst_opt_max, pool);
		opt.End();
		// compare the results
		bool n_res = TestCompare(dst_ref_min, dst_opt_min, threshold);
		n_res &= TestCompare(dst_ref_max, dst_opt_max, threshold);

		PrintTestResults(n_res, status, ref, opt, col_handle);

		if (!n_res)
		{
			num_failed_cases++;
		}

		res &= n_res;
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

	return res;
}