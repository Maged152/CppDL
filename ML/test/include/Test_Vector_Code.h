#pragma once

#include "vector.h"
#include <numbers>

namespace test
{
	 void TestVectorAdd(const qlm::Vector& src1, const qlm::Vector& src2, qlm::Vector& dst);
	///////////////////////////////////////////////////////////////////////////
	 void TestVectorDiv(const qlm::Vector& src1, const qlm::Vector& src2, qlm::Vector& dst);
	///////////////////////////////////////////////////////////////////////////
	 void TestVectorDot(const qlm::Vector& src1, const qlm::Vector& src2, float& dst);
	///////////////////////////////////////////////////////////////////////////
	 void TestVectorMag(const qlm::Vector& src, float& dst);
	///////////////////////////////////////////////////////////////////////////
	 void TestVectorAngle(const qlm::Vector& src1, const qlm::Vector& src2, float& angle);
	///////////////////////////////////////////////////////////////////////////
	 void TestVectorUnit(const qlm::Vector& src, qlm::Vector& dst);
	///////////////////////////////////////////////////////////////////////////
	 void TestVectorMul(const qlm::Vector& src1, const qlm::Vector& src2, qlm::Vector& dst);
	///////////////////////////////////////////////////////////////////////////
	 void TestVectorScalarAdd(const qlm::Vector& src1, const float val, qlm::Vector& dst);
	///////////////////////////////////////////////////////////////////////////
	 void TestVectorScalarDiv(const qlm::Vector& src1, const float val, qlm::Vector& dst);
	///////////////////////////////////////////////////////////////////////////
	 void TestVectorScalarMul(const qlm::Vector& src1, const float val, qlm::Vector& dst);
	///////////////////////////////////////////////////////////////////////////
	 void TestVectorScalarSub(const qlm::Vector& src1, const float val, qlm::Vector& dst);
	///////////////////////////////////////////////////////////////////////////
	 void TestVectorSub(const qlm::Vector& src1, const qlm::Vector& src2, qlm::Vector& dst);
	///////////////////////////////////////////////////////////////////////////
	 void TestVectorSum(const qlm::Vector& src, float& dst);
	 ///////////////////////////////////////////////////////////////////////////
	 void TestVectorMean(const qlm::Vector& src, float& dst);
	 ///////////////////////////////////////////////////////////////////////////
	 void TestVectorVar(const qlm::Vector& src, float& dst);
	 ///////////////////////////////////////////////////////////////////////////
	 void TestVectorCov(const qlm::Vector& src1, const qlm::Vector& src2, float& dst);
	 ///////////////////////////////////////////////////////////////////////////
	 void TestVectorCorr(const qlm::Vector& src1, const qlm::Vector& src2, float& dst);
	 //////////////////////////////////////////////////////////////////////////
}