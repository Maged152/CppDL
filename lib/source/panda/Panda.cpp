#include "panda.h"
#include <cmath>
#include <iostream>

namespace qlm
{
	size_t Panda::Rows() const
	{
		return rows;
	}

	size_t Panda::Cols() const
	{
		return cols;
	}

	std::vector<std::string> Panda::Headers() const
	{
		return headers;
	}

	std::vector<DataType> Panda::DTypes() const
	{
		return dtypes;
	}

	void Panda::Print(size_t num_row) const
	{
		num_row = std::min(num_row, rows);

		const size_t mum_sep = cols + 1; // separators between columns
		size_t width{ mum_sep };

		for (int i = 0; i < cols; i++)
		{
			width += max_len[i];
		}

		std::string dash_line(width, '-');

		std::cout << dash_line << "\n";

		// helper print functions
		auto print_n_spaces = [](int n)
		{
			for (int i = 0; i < n; i++)
			{
				std::cout << ' ';
			}
		};

		auto print_element = [&](const std::string& str, const int index)
		{
			std::cout << "|";
			std::cout << str;

			const int num_spaces = max_len[index] - str.size();
			print_n_spaces(num_spaces);
		};


		// print headers
		for (int i = 0; i < cols; i++)
		{
			print_element(headers[i], i);
		}
		std::cout << "|\n";
		std::cout << dash_line << "\n";

		// print data
		for (int r = 0; r < num_row; r++)
		{
			for (int c = 0; c < cols; c++)
			{
				print_element(categorical_data[c][r], c);
			}
			std::cout << "|\n";
		}

		std::cout << dash_line << "\n";
	}
}