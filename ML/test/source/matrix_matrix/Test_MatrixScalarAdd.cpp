#include "../../include/matrix/Test_Matrix.h"
#include "../../include/test_utils.h"

using namespace test;
using namespace qlm;
using namespace std;

void TestMatrixAdd(const Matrix& src1, const float val, Matrix& dst)
{
	for (int r = 0; r < src1.Rows(); r++)
	{
		for (int c = 0; c < src1.Columns(); c++)
		{
			float res = src1.Get(r, c) + val;
			dst.Set(r, c, res);
		}
	}
}

void test::Test_MatrixScalarAdd(std::vector<int>& mat_rows, std::vector<int>& mat_cols, float utilization, float threshold, float min, float max)
{
	HANDLE col_handle;
	col_handle = GetStdHandle(STD_OUTPUT_HANDLE);

	if (mat_rows.size() == 0 || mat_cols.size() == 0)
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
	cout << "Matrix scalar addition test\n";
	cout << "Number of test cases = " << mat_rows.size() * mat_cols.size() << "\n";

	// add two matrix
	for (int r = 0; r < mat_rows.size(); r++)
	{
		for (int c = 0; c < mat_cols.size(); c++)
		{
			int row = mat_rows[r];
			int col = mat_cols[c];

			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_GREEN);
			cout << "Rows  : " << row << "\n" << "Cols  : " << col << "\n";

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
			TestMatrixAdd(src1, val, dst_ref);
			ref.End();
			// add matrix function
			opt.Start();
			auto status = src1.Add(val, dst_opt, utilization);
			opt.End();
			// compare the results
			bool res = TestCompare(dst_ref, dst_opt, threshold);

			PrintTestResults(res, status, ref, opt, col_handle);
		
		}
	}
	SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_WHITE);
}