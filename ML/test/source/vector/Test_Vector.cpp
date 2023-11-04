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

	res &= Test_VectorAdd(sizes);
	res &= Test_VectorSub(sizes);
	res &= Test_VectorDiv(sizes);
	res &= Test_VectorMul(sizes);

	res &= Test_VectorDot(sizes);
	res &= Test_VectorMag(sizes);
	res &= Test_VectorAngle(sizes); 
	res &= Test_VectorUnit(sizes);

	res &= Test_VectorSum(sizes);
	res &= Test_VectorMean(sizes);
	res &= Test_VectorVar(sizes);
	res &= Test_VectorCov(sizes);
	res &= Test_VectorCorr(sizes);

	return res;
}