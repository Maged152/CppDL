#pragma once
#include<vector>

namespace test
{
	// Matrix - Martix
	// add
	void Test_MatrixAdd(std::vector<int>& mat_width,
		std::vector<int>& mat_height,
		float utilization = 0.5f,
		float threshold = 0.001f,
		float min = 0.0f,
		float max = 1.0f);
	// sub
	void Test_Matrixsub(std::vector<int>& mat_width,
		std::vector<int>& mat_height,
		float utilization = 0.5f,
		float threshold = 0.001f,
		float min = 0.0f,
		float max = 1.0f);
	// mul
	void Test_MatrixMul(std::vector<int>& mat_width,
		std::vector<int>& mat_height,
		float utilization = 0.5f,
		float threshold = 0.001f,
		float min = 0.0f,
		float max = 1.0f);
	// div
	void Test_MatrixDiv(std::vector<int>& mat_width,
		std::vector<int>& mat_height,
		float utilization = 0.5f,
		float threshold = 0.001f,
		float min = 0.0f,
		float max = 1.0f);
	// matrix multiplication
	void Test_MatrixDot(std::vector<int>& mat_width,
		std::vector<int>& mat_height,
		float utilization = 0.5f,
		float threshold = 0.001f,
		float min = 0.0f,
		float max = 1.0f);
	// matrix transpose
	void Test_MatrixTranspose(std::vector<int>& mat_width,
		std::vector<int>& mat_height,
		float utilization = 0.5f,
		float threshold = 0.001f,
		float min = 0.0f,
		float max = 1.0f);
	////////////////////////////////////////////////////////////////
	// Matrix - Scalar
	////////////////////////////////////////////////////////////////
	// Matrix - Vector
}