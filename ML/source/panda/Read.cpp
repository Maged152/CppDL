#include "panda.h"
#include <fstream>
#include <sstream>
#include <iostream>

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

		int num_rows = 0;

		std::string line;
		
		if (header)
		{
			// read header
			std::getline(file, line);
			auto push_str = [](const std::string& token, std::vector<std::string>& tokens)
			{
				tokens.push_back(token);
			};

			parse(line, this->headers, push_str);
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
		
		parse(line, this->dtypes, deduce_dtypes);

		cols = dtypes.size();

		file.close();

		return Status::SUCCESS;
	}
}

