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
	/////////////////////////////////////////////////////////////
	// vector operations
	// dot product
	void Test_VectorDot(std::vector<int>& vec_len,
		float utilization = 0.5f,
		float threshold = 0.001f,
		float min = 0.0f,
		float max = 1.0f);
	// magnitude
	void Test_VectorMag(std::vector<int>& vec_len,
		float utilization = 0.5f,
		float threshold = 0.001f,
		float min = 0.0f,
		float max = 1.0f);
	// unit vector
	void Test_VectorUnit(std::vector<int>& vec_len,
		float utilization = 0.5f,
		float threshold = 0.001f,
		float min = 0.0f,
		float max = 1.0f);
	// angle
	void Test_VectorAngle(std::vector<int>& vec_len,
		float utilization = 0.5f,
		float threshold = 0.001f,
		float min = 0.0f,
		float max = 1.0f);
	// transform using transformation matrix
	void Test_VectorTransform(std::vector<int>& vec_len,
		float utilization = 0.5f,
		float threshold = 0.001f,
		float min = 0.0f,
		float max = 1.0f);
}