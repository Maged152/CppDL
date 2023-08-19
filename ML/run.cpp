#include "shakhbat_ml.h"
#include <iostream>
#include "test/test.h"
#include <vector>

int main()
{
	std::vector<int>l {10, 50, 200, 2000, 2000};
	test::Test_VectorAdd(l, 1);

	/*qlm::Vector v1{ 10 };
	qlm::Vector v2{ 10 };
	qlm::Vector v3{ 10 };

	v1.RandomInit(0, 10);
	v2.RandomInit(0, 10);

	std::cout << "\nv1\n";
	v1.Print();
	std::cout << "\nv2\n";
	v2.Print();

	v1.Add(v2, v3);

	std::cout << "\nv3\n";
	v3.Print();*/

}