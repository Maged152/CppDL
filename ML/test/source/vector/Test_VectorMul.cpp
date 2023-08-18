#include "../../include/Test_Vector.h"
#include "../../include/test_utils.h"
#include "shakhbat_ml.h"
#include <iostream>
#include <windows.h>

using namespace test;
using namespace qlm;
using namespace std;

void TestVectorMul(const Vector& src1, const Vector& src2, Vector& dst)
{
	for (int l = 0; l < src1.Length(); l++)
	{
		float res = src1.Get(l) * src2.Get(l);
		dst.Set(l, res);
	}
}

void test::Test_VectorMul(std::vector<int>& vec_len, float utilization, float threshold, float min, float max)
{
	HANDLE col_handle;
	col_handle = GetStdHandle(STD_OUTPUT_HANDLE);

	if (vec_len.size() == 0)
	{
		SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_LIGHT_RED);
		cout << "Dimensions array must have valus\n";
		return;
	}

	if (max <= min)
	{
		SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_LIGHT_RED);
		cout << "Max value must be greater than the min value\n";
		return;
	}

	SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_BLUE);
	cout << "Vector Mul test\n";
	cout << "Number of test cases = " << vec_len.size() << "\n";

	// Mul two vector
	for (int l = 0; l < vec_len.size(); l++)
	{

		int len = vec_len[l];

		SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_GREEN);
		cout << "lenght  : " << len << "\n";

		Timer<usec> ref;
		Timer<usec> opt;

		Vector src1{ len };
		Vector src2{ len };

		Vector dst_ref{ len };
		Vector dst_opt{ len };
		// random initialization
		src1.RandomInit(min, max);
		src2.RandomInit(min, max);
		// test Vector Mul
		ref.Start();
		TestVectorMul(src1, src2, dst_ref);
		ref.End();
		// Mul Vector function
		opt.Start();
		auto status = src1.Mul(src2, dst_opt, utilization);
		opt.End();
		// compare the results
		bool res = TestCompare(dst_ref, dst_opt, threshold);

		if (res && status == Status::SUCCESS)
		{
			// print output information
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_GREEN);
			cout << "opt code time  : " << opt.Duration() << " usec\n";
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_YELLOW);
			cout << "test code time  : " << ref.Duration() << " usec\n";

			if (opt.Duration() < ref.Duration())
			{
				SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_GREEN);
				cout << "opt code is faster by  : " << ((ref.Duration() - opt.Duration()) / opt.Duration()) * 100 << " %\n";
			}
			else
			{
				SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_LIGHT_YELLOW);
				cout << "opt code is slower by  : " << ((opt.Duration() - ref.Duration()) / ref.Duration()) * 100 << " %\n";
			}

			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_GREEN);
			cout << "STATUS  : " << "PASSED\n";
		}
		else
		{
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_LIGHT_RED);
			cout << "STATUS  : " << "FAILED\n";
		}

	}
	SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_WHITE);
}