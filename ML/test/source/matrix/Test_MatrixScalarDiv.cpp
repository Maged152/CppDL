#include "../../test.h"

using namespace test;
using namespace qlm;
using namespace std;


void test::Test_MatrixScalarDiv(std::vector<int>& mat_rows, std::vector<int>& mat_cols, int num_threads, float threshold, float min, float max)
{
	HANDLE col_handle;
	col_handle = GetStdHandle(STD_OUTPUT_HANDLE);

	if (mat_rows.size() == 0 || mat_cols.size() == 0)
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
	cout << "Matrix scalar sub test\n";
	cout << "Number of test cases = " << mat_rows.size() * mat_cols.size() << "\n";

	int num_failed_cases = 0;

	// add two matrix
	for (int r = 0; r < mat_rows.size(); r++)
	{
		for (int c = 0; c < mat_cols.size(); c++)
		{
			int row = mat_rows[r];
			int col = mat_cols[c];

			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_GREEN);
			cout << "Rows  : " << row << "\n" << "Cols  : " << col << "\n";
			
			ThreadPool pool{ num_threads };

			Timer<usec> ref;
			Timer<usec> opt;

			Matrix src1{ row, col };

			Matrix dst_ref{ row, col };
			Matrix dst_opt{ row, col };
			// random initialization
			src1.RandomInit(min, max);
			// random value for the operation
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<float> dis(min, max);
			const float val = dis(gen);
			// test matrix addition
			ref.Start();
			TestMatrixDiv(src1, val, dst_ref);
			ref.End();
			// add matrix function
			opt.Start();
			auto status = src1.Div(val, dst_opt, pool);
			opt.End();
			// compare the results
			bool res = TestCompare(dst_ref, dst_opt, threshold);

			PrintTestResults(res, status, ref, opt, col_handle);

			if (!res)
			{
				num_failed_cases++;
			}

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