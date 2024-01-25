#include "panda.h"
#include <fstream>

namespace qlm
{
	Status Panda::Write(const std::string& file_name, const bool header, const char& sep) const
	{
		// Create an output file stream object
		std::ofstream file(file_name);

		if (!file.is_open()) 
		{
			return Status::FAIL;
		}

		// Write header
		if (header)
		{
			for (const auto& header : headers) 
			{
				file << header;
				if (&header != &headers.back()) file << sep;
			}
		
			file << "\n";
		}

		// Write data
		for (int r = 0; r < rows; r++)
		{
			for (int c = 0; c < cols; c++)
			{
				file << categorical_data[c][r];
				if (c != cols - 1) file << sep; // No comma at end of line
			}
			file << "\n";
		}


		// Close the file
		file.close();

		return Status::SUCCESS;
	}
}