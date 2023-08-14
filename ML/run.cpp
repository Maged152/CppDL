#include "shakhbat_ml.h"
#include <iostream>
#include "test/test.h"
#include <vector>

int main()
{
	std::vector<int>w {10, 50, 200, 2000};
	std::vector<int>h { 10, 50, 200, 2000};
	test::Test_MatrixScalarAdd(w, h, 0.5);
	/*qlm::Matrix m1{ 3, 2 };
	qlm::Matrix m2{ 2, 3 };
	qlm::Matrix m3{ 3, 3 };*/

	/*m1.RandomInit(0, 10);
	m2.RandomInit(0, 10);

	std::cout << "m1\n";
	m1.Print();
	std::cout << "m2\n";
	m2.Print();

	m1.Dot(m2, m3);

	std::cout << "m3\n";
	m3.Print();*/

}