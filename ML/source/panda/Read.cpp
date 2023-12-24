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

		auto parse = [&sep](const std::string& line, std::vector<std::string>& tokens, auto& fun)
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
		std::getline(file, line);

		// check if the input is float
		auto is_float = [](std::string& str)
		{
			// erase trailing spaces
			const std::string white_space = " \r\n\t\v\f";
			str.erase(str.find_last_not_of(white_space) + 1);

			bool result = false;

			try 
			{
				size_t pos;
				const float num = std::stof(str, &pos);
				if (pos == str.size())
				{
					result = true;
				}
					
			}
			catch (...) 
			{}

			return result;
		};

		// deduce the data types
		auto deduce_dtypes = [&](std::string& token, std::vector<std::string>& tokens)
		{
			if (is_float(token))
			{
				tokens.push_back("numerical");
			}
			else
			{
				tokens.push_back("categorical");
			}

		};
		
		parse(line, dtypes, deduce_dtypes);

		cols = dtypes.size();

		// set sizes of the vectors
		categorical_data = std::vector<std::vector<std::string>>{ cols };
		numerical_data = std::vector<qlm::Vector>{ cols };
		max_len = std::vector<unsigned int>{ cols, 1 };

		// set headers to numbers if not exist
		if (!header)
		{
			for (int i = 0; i < cols; i++)
			{
				headers.push_back(std::to_string(i));
			}
		}
		else
		{
			for (int i = 0; i < cols; i++)
			{
				max_len[i] = headers[i].size();
			}
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

