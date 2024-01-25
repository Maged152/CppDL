#include "shakhbat_dl.h"
#include <iostream>
#include <vector>


int main()
{
	qlm::Panda pd;
	pd.Read("cars.csv");

	pd.Print();

	pd.Write("mycsv.csv");
	std::cout << pd.Cols() << "\n";
	

}

