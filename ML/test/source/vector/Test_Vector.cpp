#include "../test/include/Test_Vector.h"
#include "shakhbat_ml.h"

using namespace test;

bool test::Test_Vector()
{
	std::vector<int> sizes{ 100, 5000, 20000, 200000, 2000000 };

	bool res = true;
	// vector tests
	res &= Test_VectorScalarAdd(sizes);
	res &= Test_VectorScalarSub(sizes);
	res &= Test_VectorScalarDiv(sizes);
	res &= Test_VectorScalarMul(sizes);

	return res;
}