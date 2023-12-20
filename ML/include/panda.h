#pragma once
#include "vector.h"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

namespace qlm
{
	class Panda
	{
	private:
		int cols;
		int rows;

		std::vector<std::string> headers;
		std::vector<std::string> str_data;
		std::vector<qlm::Vector> num_data;
	public:
		Panda()
		{}

		~Panda();

	public:
		Status Read(const std::string& file_name, const std::string& sep);
		Status Write(const std::string& file_name, const std::string& sep);

		void Print(int num_row = 10);
	};
}