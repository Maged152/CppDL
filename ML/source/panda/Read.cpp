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

		auto parse = [&sep](const std::string& line, std::vector<std::string>& tokens)
		{
			std::stringstream ss(line);
			std::string token;

			while (std::getline(ss, token, sep)) 
			{
				tokens.push_back(token);
			}

		};

		int num_rows = 0;

		std::string line;
		
		if (header)
		{
			// read header
			std::getline(file, line);
			parse(line, this->headers);
		}
		

		file.close();

		return Status::SUCCESS;
	}
}