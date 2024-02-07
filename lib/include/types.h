#pragma once

namespace qlm
{
	enum class Status
	{
		SUCCESS,
		FAIL,
		INVALID_DIMENSIONS,
		INVALID_UTILIZATION,
		FILE_NOT_FOUND,
		EMPTY_FILE
	};

	enum class BroadCast
	{
		BROAD_CAST_ROW,
		BROAD_CAST_COLUMN
	};

	enum class Norm
	{
		L1_NORM,
		L2_NORM,
		INF_NORM
	};

	enum class DataType
	{
		CATEGORICAL,
		NUMERICAL
	};
}