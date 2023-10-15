#include "../../test.h"
#include "shakhbat_ml.h"
#include <iostream>
#include <windows.h>

using namespace test;
using namespace qlm;
using namespace std;


void TestMatrixVectorDiv_Coloumn(const Matrix& src1, const Vector& src2, Matrix& dst)
{
	for (int r = 0; r < src1.Rows(); r++)
	{
		for (int c = 0; c < src1.Columns(); c++)
		{
			float res = src1.Get(r, c) / src2.Get(c);
			dst.Set(r, c, res);
		}
	}
}

void test::Test_MatrixVectorDiv(std::vector<int>& mat_rows, std::vector<int>& mat_cols, int num_threads, float threshold, float min, float max)
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
	cout << "Matrix Vector div test\n";
	cout << "Number of test cases = " << mat_rows.size() * mat_cols.size() << "\n";

	int num_failed_cases = 0;
	BroadCast broad_cast = BroadCast::BROAD_CAST_ROW;

	// div vector matrix
	for (int r = 0; r < mat_rows.size(); r++)
	{
		for (int c = 0; c < mat_cols.size(); c++)
		{
			int row = mat_rows[r];
			int col = mat_cols[c];
			int vector_length = (broad_cast == BroadCast::BROAD_CAST_ROW) ? row : col;

			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_GREEN);
			cout << "Rows  : " << row << "\n" << "Cols  : " << col << "\n";
			cout << "Length  : " << vector_length << "\n";

			ThreadPool pool{ num_threads };

			Timer<usec> ref;
			Timer<usec> opt;

			Matrix src1{ row, col };
			Vector src2{ vector_length };

			Matrix dst_ref{ row, col };
			Matrix dst_opt{ row, col };
			// random initialization
			src1.RandomInit(min, max);
			src2.RandomInit(min, max);
			// test matrix addition
			if (broad_cast == BroadCast::BROAD_CAST_ROW)
			{
				ref.Start();
				TestMatrixVectorDiv_Row(src1, src2, dst_ref);
				ref.End();
			}
			else
			{
				ref.Start();
				TestMatrixVectorDiv_Coloumn(src1, src2, dst_ref);
				ref.End();
			}
			// div matrix function
			opt.Start();
			auto status = src1.Div(src2, dst_opt, broad_cast, pool);
			opt.End();
			// compare the results
			bool res = TestCompare(dst_ref, dst_opt, threshold);

			PrintTestResults(res, status, ref, opt, col_handle);

			if (!res)
			{
				num_failed_cases++;
			}

			broad_cast = (broad_cast == BroadCast::BROAD_CAST_ROW) ? BroadCast::BROAD_CAST_COLUMN : BroadCast::BROAD_CAST_ROW;
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