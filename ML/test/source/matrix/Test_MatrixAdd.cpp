#include "../../include/matrix/Test_MatrixAdd.h"
#include "../../include/test_utils.h"
#include "shakhbat_ml.h"
#include <iostream>
#include <windows.h>

using namespace test;
using namespace qlm;
using namespace std;

void TestMatrixAdd(const Matrix& src1, const Matrix& src2, Matrix& dst)
{
	for (int i = 0; i < src1.Width(); i++)
	{
		for (int j = 0; j < src1.Height(); j++)
		{
			float res = src1.Get(i, j) + src2.Get(i, j);
			dst.Set(i, j, res);
		}
	}
}

void test::Test_MatrixAdd(std::vector<int>& mat_width, std::vector<int>& mat_height, float threshold, float min, float max)
{
	HANDLE col;
	col = GetStdHandle(STD_OUTPUT_HANDLE);

	if (mat_width.size() == 0 || mat_height.size() == 0)
	{
		SetConsoleTextAttribute(col, CONSOLE_COLOR_LIGHT_RED);
		cout << "Dimensions array must have valus\n";
		return;
	}

	if (max <= min)
	{
		SetConsoleTextAttribute(col, CONSOLE_COLOR_LIGHT_RED);
		cout << "Max value must be greater than the min value\n";
		return;
	}

	SetConsoleTextAttribute(col, CONSOLE_COLOR_BLUE);
	cout << "Matrix addition test\n";
	cout << "Number of test cases = " << mat_width.size() * mat_height.size() << "\n";

	// add two matrix
	for (int w = 0; w < mat_width.size(); w++)
	{
		for (int h = 0; h < mat_height.size(); h++)
		{
			int width = mat_width[w];
			int height = mat_height[h];

			SetConsoleTextAttribute(col, CONSOLE_COLOR_GREEN);
			cout << "Width  : " << width << "\n" << "Height  : " << height << "\n";

			Timer<usec> ref;
			Timer<usec> opt;

			Matrix src1{width, height};
			Matrix src2{width, height};

			Matrix dst_ref{width, height};
			Matrix dst_opt{width, height};
			// random initialization
			src1.RandomInit(min, max);
			src2.RandomInit(min, max);
			// test matrix addition
			ref.Start();
			TestMatrixAdd(src1, src2, dst_ref);
			ref.End();
			// add matrix function
			opt.Start();
			auto status = src1.MatrixAdd(src2, dst_ref, 1.0f);
			opt.End();
			// compare the results
			bool res = TestCompare(dst_ref, dst_opt, threshold);

			if (res && status == Status::SUCCESS)
			{
				// print output information
				SetConsoleTextAttribute(col, CONSOLE_COLOR_GREEN);
				cout << "opt code time  : " << opt.Duration() << " usec\n";
				SetConsoleTextAttribute(col, CONSOLE_COLOR_YELLOW);
				cout << "test code time  : " << ref.Duration() << " usec\n";

				if (opt.Duration() < ref.Duration())
				{
					SetConsoleTextAttribute(col, CONSOLE_COLOR_GREEN);
					cout << "opt code is faster by  : " << (opt.Duration() / ref.Duration()) * 100<< " %\n";
				}
				else
				{
					SetConsoleTextAttribute(col, CONSOLE_COLOR_RED);
					cout << "opt code is slower by  : " << (ref.Duration() / opt.Duration()) * 100 << " %\n";
				}

				SetConsoleTextAttribute(col, CONSOLE_COLOR_GREEN);
				cout << "STATUS  : " << "PASSED\n";
			}
			else
			{
				SetConsoleTextAttribute(col, CONSOLE_COLOR_LIGHT_RED);
				cout << "STATUS  : " << "FAILED\n";
			}
		}
	}
}