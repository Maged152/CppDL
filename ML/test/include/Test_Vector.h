#pragma once
#include<vector>

namespace test
{
	// Vector - Vector
	// add
	void Test_VectorAdd(std::vector<int>& vec_len,
		float utilization = 0.5f,
		float threshold = 0.001f,
		float min = 0.0f,
		float max = 1.0f);
	// sub
	void Test_VectorSub(std::vector<int>& vec_len,
		float utilization = 0.5f,
		float threshold = 0.001f,
		float min = 0.0f,
		float max = 1.0f);
	// mul
	void Test_VectorMul(std::vector<int>& vec_len,
		float utilization = 0.5f,
		float threshold = 0.001f,
		float min = 0.0f,
		float max = 1.0f);
	// div
	void Test_VectorDiv(std::vector<int>& vec_len,
		float utilization = 0.5f,
		float threshold = 0.001f,
		float min = 0.0f,
		float max = 1.0f);
	////////////////////////////////////////////////////////////////
	// Vector - Scalar
	void Test_VectorScalarAdd(std::vector<int>& vec_len,
		float utilization = 0.5f,
		float threshold = 0.001f,
		float min = 0.0f,
		float max = 1.0f);
	// sub
	void Test_VectorScalarSub(std::vector<int>& vec_len,
		float utilization = 0.5f,
		float threshold = 0.001f,
		float min = 0.0f,
		float max = 1.0f);
	// mul
	void Test_VectorScalarMul(std::vector<int>& vec_len,
		float utilization = 0.5f,
		float threshold = 0.001f,
		float min = 0.0f,
		float max = 1.0f);
	// div
	void Test_VectorScalarDiv(std::vector<int>& vec_len,
		float utilization = 0.5f,
		float threshold = 0.001f,
		float min = 0.0f,
		float max = 1.0f);
}