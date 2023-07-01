#include "../../include/matrix/matrix_add.h"
#include "../../include/console_color.h"
#include "matrix.h"
#include <iostream>
#include <windows.h>

using namespace test;
using namespace qlm;
using namespace std;

void Test_MatrixAdd(vector<int> mat_width, vector<int> mat_height, float threshold, float min, float max)
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

			Matrix src1{w, h};
			Matrix src2{w, h};

			Matrix dst_ref{w, h};
			Matrix dst{w, h};
			// random initialization
			
			// matrix addition
			for (int i = 0; i < src1.Width(); i++)
			{
				for (int j = 0; j < src1.Height(); j++)
				{
					float res = src1.Get(i, j) + src2.Get(i, j);
					dst_ref.Set(i, j, res);
				}
			}
			// call add matrix function

			// compare the results
		}
	}
}