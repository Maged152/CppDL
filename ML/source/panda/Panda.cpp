#include "panda.h"

namespace qlm
{
	unsigned int Panda::Rows() const
	{
		return rows;
	}

	unsigned int Panda::Cols() const
	{
		return cols;
	}

	std::vector<std::string> Panda::Headers() const
	{
		return headers;
	}

	std::vector<std::string> Panda::DTypes() const
	{
		return dtypes;
	}


}