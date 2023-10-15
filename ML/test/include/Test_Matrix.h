#pragma once

#include "Test_Matrix_Code.h"
#include <vector>
#include <thread>
namespace test
{
	// Matrix - Matrix
	// add
	void Test_MatrixAdd(std::vector<int>& mat_width,
		std::vector<int>& mat_height,
		int num_threads = std::thread::hardware_concurrency(),
		float threshold = 0.001f,
		float min = 0.0f,
		float max = 1.0f);
	// sub
	void Test_MatrixSub(std::vector<int>& mat_width,
		std::vector<int>& mat_height,
		int num_threads = std::thread::hardware_concurrency(),
		float threshold = 0.001f,
		float min = 0.0f,
		float max = 1.0f);
	// mul
	void Test_MatrixMul(std::vector<int>& mat_width,
		std::vector<int>& mat_height,
		int num_threads = std::thread::hardware_concurrency(),
		float threshold = 0.001f,
		float min = 0.0f,
		float max = 1.0f);
	// div
	void Test_MatrixDiv(std::vector<int>& mat_width,
		std::vector<int>& mat_height,
		int num_threads = std::thread::hardware_concurrency(),
		float threshold = 0.001f,
		float min = 0.0f,
		float max = 1.0f);
	// matrix multiplication
	void Test_MatrixDot(std::vector<int>& mat_width,
		std::vector<int>& mat_height,
		int num_threads = std::thread::hardware_concurrency(),
		float threshold = 0.001f,
		float min = 0.0f,
		float max = 1.0f);
	// matrix transpose
	void Test_MatrixTranspose(std::vector<int>& mat_width,
		std::vector<int>& mat_height,
		int num_threads = std::thread::hardware_concurrency(),
		float threshold = 0.001f,
		float min = 0.0f,
		float max = 1.0f);
	////////////////////////////////////////////////////////////////
	// Matrix - Scalar
	void Test_MatrixScalarAdd(std::vector<int>& mat_width,
		std::vector<int>& mat_height,
		int num_threads = std::thread::hardware_concurrency(),
		float threshold = 0.001f,
		float min = 0.0f,
		float max = 1.0f);
	// sub
	void Test_MatrixScalarSub(std::vector<int>& mat_width,
		std::vector<int>& mat_height,
		int num_threads = std::thread::hardware_concurrency(),
		float threshold = 0.001f,
		float min = 0.0f,
		float max = 1.0f);
	// mul
	void Test_MatrixScalarMul(std::vector<int>& mat_width,
		std::vector<int>& mat_height,
		int num_threads = std::thread::hardware_concurrency(),
		float threshold = 0.001f,
		float min = 0.0f,
		float max = 1.0f);
	// div
	void Test_MatrixScalarDiv(std::vector<int>& mat_width,
		std::vector<int>& mat_height,
		int num_threads = std::thread::hardware_concurrency(),
		float threshold = 0.001f,
		float min = 0.0f,
		float max = 1.0f);
	////////////////////////////////////////////////////////////////
	// Matrix - Vector
	// add
	void Test_MatrixVectorAdd(std::vector<int>& mat_width,
		std::vector<int>& mat_height,
		int num_threads = std::thread::hardware_concurrency(),
		float threshold = 0.001f,
		float min = 0.0f,
		float max = 1.0f);
	// sub
	void Test_MatrixVectorSub(std::vector<int>& mat_width,
		std::vector<int>& mat_height,
		int num_threads = std::thread::hardware_concurrency(),
		float threshold = 0.001f,
		float min = 0.0f,
		float max = 1.0f);
	// mul
	void Test_MatrixVectorMul(std::vector<int>& mat_width,
		std::vector<int>& mat_height,
		int num_threads = std::thread::hardware_concurrency(),
		float threshold = 0.001f,
		float min = 0.0f,
		float max = 1.0f);
	// div
	void Test_MatrixVectorDiv(std::vector<int>& mat_width,
		std::vector<int>& mat_height,
		int num_threads = std::thread::hardware_concurrency(),
		float threshold = 0.001f,
		float min = 0.0f,
		float max = 1.0f);
}