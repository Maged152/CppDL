#pragma once

namespace qlm
{
	enum class Status
	{
		SUCCESS,
		FAIL,
		INVALID_DIMENTIONS,
		INVALID_UTILIZATION	
	};

	enum class BroadCast
	{
		BROAD_CAST_ROW,
		BROAD_CAST_COLUMN
	};

	enum class NORM
	{
		L1_NORM,
		L2_NORM,
		INF_NORM
	};
}