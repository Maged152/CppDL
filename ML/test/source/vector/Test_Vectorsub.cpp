#include "../../test.h"
#include "shakhbat_ml.h"
#include <iostream>
#include <windows.h>

using namespace test;
using namespace qlm;
using namespace std;


bool test::Test_VectorSub(std::vector<int>& vec_len, unsigned int  num_threads, float threshold, float min, float max)
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
	cout << "Vector Sub test\n";
	cout << "Number of test cases = " << vec_len.size() << "\n";

	int num_failed_cases = 0;
	bool res = true;

	// Sub two vector
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
		// test Vector sub
		ref.Start();
		TestVectorSub(src1, src2, dst_ref);
		ref.End();
		// Sub Vector function
		opt.Start();
		auto status = src1.Sub(src2, dst_opt, pool);
		opt.End();
		// compare the results
		bool n_res = TestCompare(dst_ref, dst_opt, threshold);

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