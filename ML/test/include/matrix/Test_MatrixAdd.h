#pragma once
#include<vector>

namespace test
{
	// add ranges for initialization
	void Test_MatrixAdd(std::vector<int>& mat_width,
		std::vector<int>& mat_height,
		float threshold = 0.001f,
		float min = 0.0f,
		float max = 1.0f);
}