#pragma once
#include "vector.h"
#include <string>
#include <vector>

namespace qlm
{
	class Panda
	{
	private:
		size_t cols;
		size_t rows;

		std::vector<std::string> headers;
		std::vector<DataType> dtypes;  

		std::vector<std::vector<std::string>> categorical_data;
		std::vector<qlm::Vector> numerical_data;

		std::vector<size_t> max_len;
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

		void Print(size_t num_row = 10) const;

		size_t Rows() const;
		size_t Cols() const;

		std::vector<std::string> Headers() const;
		std::vector<DataType> DTypes() const;
	};
}