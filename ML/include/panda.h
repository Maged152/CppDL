#pragma once
#include "vector.h"
#include <string>
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
		Panda() : cols(0), rows(0)
		{}

		~Panda()
		{
			rows = 0;
			cols = 0;
			// TODO
		}

	public:
		Status Read(const std::string& file_name, const bool header = true, const char& sep = ',');
		Status Write(const std::string& file_name, const bool header = true, const char& sep = ',') const;

		void Print(int num_row = 10) const;

		int Rows() const;
		int Cols() const;
		std::vector<std::string> Headers() const;
	};
}