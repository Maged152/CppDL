#pragma once

#include "Test_Vector_Code.h"
#include <vector>
#include <thread>

namespace test
{
	// run all
	bool Test_Vector();
	// Vector - Vector
	// add
	bool Test_VectorAdd(std::vector<int>& vec_len,
		unsigned int  num_threads = std::thread::hardware_concurrency(),
		float threshold = 0,
		float min = 0.0f,
		float max = 100.0f);
	// sub
	bool Test_VectorSub(std::vector<int>& vec_len,
		unsigned int  num_threads = std::thread::hardware_concurrency(),
		float threshold = 0,
		float min = 0.0f,
		float max = 100.0f);
	// mul
	bool Test_VectorMul(std::vector<int>& vec_len,
		unsigned int  num_threads = std::thread::hardware_concurrency(),
		float threshold = 0,
		float min = 0.0f,
		float max = 100.0f);
	// div
	bool Test_VectorDiv(std::vector<int>& vec_len,
		unsigned int  num_threads = std::thread::hardware_concurrency(),
		float threshold = 0,
		float min = 0.0f,
		float max = 100.0f);
	////////////////////////////////////////////////////////////////
	// Vector - Scalar
	bool Test_VectorScalarAdd(std::vector<int>& vec_len,
		unsigned int  num_threads = std::thread::hardware_concurrency(),
		float threshold = 0,
		float min = 0.0f,
		float max = 100.0f);
	// sub
	bool Test_VectorScalarSub(std::vector<int>& vec_len,
		unsigned int  num_threads = std::thread::hardware_concurrency(),
		float threshold = 0,
		float min = 0.0f,
		float max = 100.0f);
	// mul
	bool Test_VectorScalarMul(std::vector<int>& vec_len,
		unsigned int  num_threads = std::thread::hardware_concurrency(),
		float threshold = 0,
		float min = 0.0f,
		float max = 100.0f);
	// div
	bool Test_VectorScalarDiv(std::vector<int>& vec_len,
		unsigned int  num_threads = std::thread::hardware_concurrency(),
		float threshold = 0,
		float min = 0.0f,
		float max = 100.0f);
	/////////////////////////////////////////////////////////////
	// vector operations
	// dot product
	bool Test_VectorDot(std::vector<int>& vec_len,
		unsigned int  num_threads = std::thread::hardware_concurrency(),
		float threshold = 0.0005f,
		float min = 0.0f,
		float max = 100.0f);
	// magnitude
	bool Test_VectorMag(std::vector<int>& vec_len,
		unsigned int  num_threads = std::thread::hardware_concurrency(),
		float threshold = 0.0005f,
		float min = 0.0f,
		float max = 100.0f);
	// unit vector
	bool Test_VectorUnit(std::vector<int>& vec_len,
		unsigned int  num_threads = std::thread::hardware_concurrency(),
		float threshold = 0.00001f,
		float min = 0.0f,
		float max = 100.0f);
	// angle
	bool Test_VectorAngle(std::vector<int>& vec_len,
		unsigned int  num_threads = std::thread::hardware_concurrency(),
		float threshold = 0.00001f,
		float min = 0.0f,
		float max = 100.0f);
	// sum
	bool Test_VectorSum(std::vector<int>& vec_len,
		unsigned int  num_threads = std::thread::hardware_concurrency(),
		float threshold = 0.00001f,
		float min = 0.0f,
		float max = 100.0f);
	// mean
	bool Test_VectorMean(std::vector<int>& vec_len,
		unsigned int  num_threads = std::thread::hardware_concurrency(),
		float threshold = 0.00001f,
		float min = 0.0f,
		float max = 100.0f);
	// variance
	bool Test_VectorVar(std::vector<int>& vec_len,
		unsigned int  num_threads = std::thread::hardware_concurrency(),
		float threshold = 0.00001f,
		float min = 0.0f,
		float max = 100.0f);
	// covariance
	bool Test_VectorCov(std::vector<int>& vec_len,
		unsigned int  num_threads = std::thread::hardware_concurrency(),
		float threshold = 0.00001f,
		float min = 0.0f,
		float max = 100.0f);
	// correlation
	bool Test_VectorCorr(std::vector<int>& vec_len,
		unsigned int  num_threads = std::thread::hardware_concurrency(),
		float threshold = 0.00001f,
		float min = 0.0f,
		float max = 100.0f);
	// min
	bool Test_VectorMin(std::vector<int>& vec_len,
		unsigned int  num_threads = std::thread::hardware_concurrency(),
		float threshold = 0.0f,
		float min = 0.0f,
		float max = 100.0f);
	// max
	bool Test_VectorMax(std::vector<int>& vec_len,
		unsigned int  num_threads = std::thread::hardware_concurrency(),
		float threshold = 0.0f,
		float min = 0.0f,
		float max = 100.0f);
	// min max
	bool Test_VectorMinMax(std::vector<int>& vec_len,
		unsigned int  num_threads = std::thread::hardware_concurrency(),
		float threshold = 0.0f,
		float min = 0.0f,
		float max = 100.0f);

}