#include "../../../include/Test_Vector_Code.h"

namespace test
{
	 void TestVectorAdd(const qlm::Vector& src1, const qlm::Vector& src2, qlm::Vector& dst)
	{
		for (int l = 0; l < src1.Length(); l++)
		{
			float res = src1.Get(l) + src2.Get(l);
			dst.Set(l, res);
		}
	}
	///////////////////////////////////////////////////////////////////////////
	 void TestVectorDiv(const qlm::Vector& src1, const qlm::Vector& src2, qlm::Vector& dst)
	{
		for (int l = 0; l < src1.Length(); l++)
		{
			float res = src1.Get(l) / src2.Get(l);
			dst.Set(l, res);
		}
	}
	///////////////////////////////////////////////////////////////////////////
	 void TestVectorDot(const qlm::Vector& src1, const qlm::Vector& src2, float& dst)
	{
		dst = 0;
		for (int l = 0; l < src1.Length(); l++)
		{
			dst += src1.Get(l) * src2.Get(l);
		}
	}
	///////////////////////////////////////////////////////////////////////////
	 void TestVectorMag(const qlm::Vector& src, float& dst)
	{
		dst = 0;
		TestVectorDot(src, src, dst);
		dst = std::sqrtf(dst);
	}
	///////////////////////////////////////////////////////////////////////////
	 void TestVectorAngle(const qlm::Vector& src1, const qlm::Vector& src2, float& angle)
	{
		// mag for src1
		float mag1 = 0;
		TestVectorMag(src1, mag1);
		// mag for src2
		float mag2 = 0;
		TestVectorMag(src2, mag2);
		// dot product
		float dot = 0;
		TestVectorDot(src1, src2, dot);

		angle = std::acosf(dot / (mag1 * mag2)) * 180.0f / std::numbers::pi;
	}
	///////////////////////////////////////////////////////////////////////////
	 void TestVectorUnit(const qlm::Vector& src, qlm::Vector& dst)
	{
		float mag = 0;
		TestVectorMag(src, mag);

		for (int l = 0; l < src.Length(); l++)
		{
			float res = src.Get(l) / mag;
			dst.Set(l, res);
		}
	}
	///////////////////////////////////////////////////////////////////////////
	 void TestVectorMul(const qlm::Vector& src1, const qlm::Vector& src2, qlm::Vector& dst)
	{
		for (int l = 0; l < src1.Length(); l++)
		{
			float res = src1.Get(l) * src2.Get(l);
			dst.Set(l, res);
		}
	}
	///////////////////////////////////////////////////////////////////////////
	 void TestVectorScalarAdd(const qlm::Vector& src1, const float val, qlm::Vector& dst)
	{
		for (int l = 0; l < src1.Length(); l++)
		{
			float res = src1.Get(l) + val;
			dst.Set(l, res);
		}
	}
	///////////////////////////////////////////////////////////////////////////
	 void TestVectorScalarDiv(const qlm::Vector& src1, const float val, qlm::Vector& dst)
	{
		for (int l = 0; l < src1.Length(); l++)
		{
			float res = src1.Get(l) / val;
			dst.Set(l, res);
		}
	}
	///////////////////////////////////////////////////////////////////////////
	 void TestVectorScalarMul(const qlm::Vector& src1, const float val, qlm::Vector& dst)
	{
		for (int l = 0; l < src1.Length(); l++)
		{
			float res = src1.Get(l) * val;
			dst.Set(l, res);
		}
	}
	///////////////////////////////////////////////////////////////////////////
	 void TestVectorScalarSub(const qlm::Vector& src1, const float val, qlm::Vector& dst)
	{
		for (int l = 0; l < src1.Length(); l++)
		{
			float res = src1.Get(l) - val;
			dst.Set(l, res);
		}
	}
	///////////////////////////////////////////////////////////////////////////
	 void TestVectorSub(const qlm::Vector& src1, const qlm::Vector& src2, qlm::Vector& dst)
	{
		for (int l = 0; l < src1.Length(); l++)
		{
			float res = src1.Get(l) - src2.Get(l);
			dst.Set(l, res);
		}
	}
	///////////////////////////////////////////////////////////////////////////
	 void TestVectorSum(const qlm::Vector& src, float& dst)
	 {
		 dst = 0;
		 for (int l = 0; l < src.Length(); l++)
		 {
			 dst += src.Get(l);
		 }
	 }
	 ///////////////////////////////////////////////////////////////////////////
	 void TestVectorMean(const qlm::Vector& src, float& dst)
	 {
		 TestVectorSum(src, dst);
		 dst /= src.Length();
	 }
	 ///////////////////////////////////////////////////////////////////////////
	 void TestVectorVar(const qlm::Vector& src, float& dst)
	 {
		 float mean;
		 TestVectorMean(src, mean);
		 dst = 0;
		 for (int l = 0; l < src.Length(); l++)
		 {
			 dst += std::powf(src.Get(l) - mean, 2);
		 }

		 dst = dst / (src.Length() - 1);
	 }
	 ///////////////////////////////////////////////////////////////////////////
	 void TestVectorCov(const qlm::Vector& src1, const qlm::Vector& src2, float& dst)
	 {
		 float mean1, mean2;
		 TestVectorMean(src1, mean1);
		 TestVectorMean(src2, mean2);

		 dst = 0;
		 for (int i = 0; i < src1.Length(); i++)
		 {
			 dst += (src1.Get(i) - mean1) * (src2.Get(i) - mean2);
		 }

		 dst = dst / (src1.Length() - 1);
	 }
	 ///////////////////////////////////////////////////////////////////////////
	 void test::TestVectorCorr(const qlm::Vector & src1, const qlm::Vector & src2, float & dst)
	 {
		 float cov, var1, var2;

		 TestVectorCov(src1, src2, cov);
		 TestVectorVar(src1, var1);
		 TestVectorVar(src2, var2);

		 dst = cov / std::sqrt(var1 * var2);
	 }
	 ///////////////////////////////////////////////////////////////////////////
	 ///////////////////////////////////////////////////////////////////////////
	 ///////////////////////////////////////////////////////////////////////////
	 ///////////////////////////////////////////////////////////////////////////

}