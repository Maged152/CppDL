#include "shakhbat_dl.h"
#include <iostream>
#include <vector>


int main()
{
	// qlm::Panda pd;
	// pd.Read("cars.csv");

	// pd.Print();

	// pd.Write("mycsv.csv");
	// std::cout << pd.Cols() << "\n";

	qlm::Vector input {3};
	input.RandomInit(0, 10);

	input.Print();
	std::cout << "\n";

	qlm::Dense layer (3, 4, qlm::Linear_AF);
	layer.Print();

	qlm::ThreadPool tp {};
	layer.Forward(input, tp);

	auto out = layer.Output();
	out.Print();
	std::cout << "\n";
}

