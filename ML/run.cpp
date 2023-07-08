#include "shakhbat_ml.h"
#include <iostream>
#include "test/test.h"
#include <vector>

int main()
{
	std::vector<int>w {10, 50, 200, 1000};
	std::vector<int>h { 10, 50, 200, 2000 };
	test::Test_MatrixAdd(w, h);
}