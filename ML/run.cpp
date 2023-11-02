#include "shakhbat_ml.h"
#include <iostream>
#include "test/test.h"
#include <vector>
#include "thread_pool.h"


int main()
{

	std::vector<int>l2{ 1000, 5000, 20000, 200000, 200000 };
	test::Test_VectorCov(l2, 8);
	
	int v;
	std::cin >> v;
	std::cout << v << "\n";

	/*qlm::Vector v1{ 10 };

	for (int i = 0; i < 10; i++)
	{
		v1.Set(i, i);
	}

	qlm::ThreadPool p;
	
	float dst;
	v1.Sum(dst, p);
	v1.Print();
	std::cout << "\n" << dst << "\n";*/
	/*qlm::Vector v1{ 10 };
	qlm::Matrix m1{ 10, 10 };
	qlm::Matrix m2{ 10, 10 };

	v1.RandomInit(0, 10);
	m1.RandomInit(0, 10);

	std::cout << "\nv1\n";
	v1.Print();

	std::cout << "\nm1\n";
	m1.Print();

	m1.Add(v1, m2);

	std::cout << "\nm2\n";
	m2.Print();*/

}

