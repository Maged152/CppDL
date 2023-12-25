#include "shakhbat_ml.h"
#include <iostream>
#include "test/test.h"
#include <vector>


int main()
{
	qlm::Panda pd;
	pd.Read("cars.csv");

	auto h = pd.Headers();
	auto dt = pd.DTypes();

	std::cout << "Headers\n";
	for (auto &i : h)
	{
		std::cout << i << ", ";
	}

	std::cout << "\nDTypes\n";
	for (auto& i : dt)
	{
		std::cout << i << ", ";
	}

	int x = pd.Cols();

	std::cout << x << "\n";
	std::cout << pd.Rows() << "\n";
	/*std::vector<int>l2{ 1000, 5000, 20000, 200000, 200000 };
	test::Test_VectorMin(l2);
	test::Test_VectorMax(l2);
	test::Test_VectorMinMax(l2);*/

	

	/*auto res = test::Test_Vector();

	if (res == true)
	{
		std::cout << "PASSED\n";
	}
	else
	{
		std::cout << "FAILED\n";
	}*/

}

