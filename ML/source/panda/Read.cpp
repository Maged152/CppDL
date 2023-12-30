#include "panda.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>

namespace qlm
{
	Status Panda::Read(const std::string& file_name, const bool header, const char& sep)
	{
		// open the file
		std::fstream file;
		file.open(file_name, std::ios::in);

		if (!file.is_open())
		{
			return Status::FILE_NOT_FOUND;
		}

		auto parse = [&sep]<typename T>(const std::string& line, std::vector<T>& tokens, auto& fun)
		{
			std::stringstream ss(line);
			std::string token;

			while (std::getline(ss, token, sep)) 
			{
				fun(token, tokens);
			}

		};

		std::string line;
		
		if (header)
		{
			// read header
			std::getline(file, line);
			auto push_str = [](const std::string& token, std::vector<std::string>& tokens)
			{
				tokens.push_back(token);
			};

			parse(line, headers, push_str);
		}

		// read first line
		if (!std::getline(file, line))
		{
			return Status::EMPTY_FILE;
		}

		// check if the input is float
		auto is_float = [](const std::string& str)
		{
			// erase trailing spaces
			const std::string white_space = " \r\n\t\v\f";
			std::string trimmed_str = str;

			trimmed_str.erase(str.find_last_not_of(white_space) + 1);

			bool result = false;

			try 
			{
				size_t pos;
				const float num = std::stof(trimmed_str, &pos);
				if (pos == trimmed_str.size())
				{
					result = true;
				}
					
			}
			catch (...) 
			{}

			return result;
		};

		// deduce the data types
		auto deduce_dtypes = [&](const std::string& token, std::vector<DataType>& tokens)
		{
			if (is_float(token))
			{
				tokens.push_back(DataType::NUMERICAL);
			}
			else
			{
				tokens.push_back(DataType::CATEGORICAL);
			}

		};
		
		parse(line, dtypes, deduce_dtypes);

		cols = dtypes.size();

		// set sizes of the vectors
		categorical_data.resize(cols);
		numerical_data.resize(cols);
		max_len.resize(cols);

		// set headers to numbers if not exist
		if (!header)
		{
			for (int i = 0; i < cols; i++)
			{
				headers.push_back(std::to_string(i));
			}
			
		}

		for (int i = 0; i < cols; i++)
		{
			max_len[i] = headers[i].size();
		}

		// Read data, line by line
		do
		{
			rows++;

			std::stringstream ss(line);
			std::string token;

			int idx = 0;

			while (std::getline(ss, token, sep))
			{
				unsigned int len = token.size();
				max_len[idx] = std::max(len, max_len[idx]);

				categorical_data[idx++].push_back(token);

			}

		} while (std::getline(file, line));

		file.close();


		return Status::SUCCESS;
	}
}

