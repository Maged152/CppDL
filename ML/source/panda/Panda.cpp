#include "panda.h"

namespace qlm
{
	int Panda::Rows() const
	{
		return rows;
	}

	int Panda::Cols() const
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