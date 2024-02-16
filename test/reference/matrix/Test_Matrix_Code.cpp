#include "Test_Matrix_Code.h"

namespace test
{
	///////////////////////////////////////////////////////////////////////////
	void Add(const qlm::Matrix& src1, const qlm::Matrix& src2, qlm::Matrix& dst)
	{
		for (int r = 0; r < src1.Rows(); r++)
		{
			for (int c = 0; c < src1.Columns(); c++)
			{
				float res = src1.Get(r, c) + src2.Get(r, c);
				dst.Set(r, c, res);
			}
		}
	}
	///////////////////////////////////////////////////////////////////////////
	void Div(const qlm::Matrix& src1, const qlm::Matrix& src2, qlm::Matrix& dst)
	{
		for (int r = 0; r < src1.Rows(); r++)
		{
			for (int c = 0; c < src1.Columns(); c++)
			{
				float res = src1.Get(r, c) / src2.Get(r, c);
				dst.Set(r, c, res);
			}
		}
	}
	///////////////////////////////////////////////////////////////////////////
	void Dot(const qlm::Matrix& src1, const qlm::Matrix& src2, qlm::Matrix& dst)
	{
		for (int r = 0; r < src1.Rows(); r++)
		{
			for (int c = 0; c < src2.Columns(); c++)
			{
				float sum = 0.0f; // Initialize a variable to store the sum of products

				for (int e = 0; e < src1.Columns(); e++)
				{
					sum += src1.Get(r, e) * src2.Get(e, c);
				}

				dst.Set(r, c, sum); // Set the result in the destination matrix
			}
		}
	}
	///////////////////////////////////////////////////////////////////////////
	void Mul(const qlm::Matrix& src1, const qlm::Matrix& src2, qlm::Matrix& dst)
	{
		for (int r = 0; r < src1.Rows(); r++)
		{
			for (int c = 0; c < src1.Columns(); c++)
			{
				float res = src1.Get(r, c) * src2.Get(r, c);
				dst.Set(r, c, res);
			}
		}
	}
	///////////////////////////////////////////////////////////////////////////
	void Add(const qlm::Matrix& src1, const float val, qlm::Matrix& dst)
	{
		for (int r = 0; r < src1.Rows(); r++)
		{
			for (int c = 0; c < src1.Columns(); c++)
			{
				float res = src1.Get(r, c) + val;
				dst.Set(r, c, res);
			}
		}
	}
	///////////////////////////////////////////////////////////////////////////
	void Div(const qlm::Matrix& src1, const float val, qlm::Matrix& dst)
	{
		for (int r = 0; r < src1.Rows(); r++)
		{
			for (int c = 0; c < src1.Columns(); c++)
			{
				float res = src1.Get(r, c) / val;
				dst.Set(r, c, res);
			}
		}
	}
	///////////////////////////////////////////////////////////////////////////
	void Mul(const qlm::Matrix& src1, const float val, qlm::Matrix& dst)
	{
		for (int r = 0; r < src1.Rows(); r++)
		{
			for (int c = 0; c < src1.Columns(); c++)
			{
				float res = src1.Get(r, c) * val;
				dst.Set(r, c, res);
			}
		}
	}
	///////////////////////////////////////////////////////////////////////////
	void Sub(const qlm::Matrix& src1, const float val, qlm::Matrix& dst)
	{
		for (int r = 0; r < src1.Rows(); r++)
		{
			for (int c = 0; c < src1.Columns(); c++)
			{
				float res = src1.Get(r, c) - val;
				dst.Set(r, c, res);
			}
		}
	}
	///////////////////////////////////////////////////////////////////////////
	void Sub(const qlm::Matrix& src1, const qlm::Matrix& src2, qlm::Matrix& dst)
	{
		for (int r = 0; r < src1.Rows(); r++)
		{
			for (int c = 0; c < src1.Columns(); c++)
			{
				float res = src1.Get(r, c) - src2.Get(r, c);
				dst.Set(r, c, res);
			}
		}
	}
	///////////////////////////////////////////////////////////////////////////
	void Add(const qlm::Matrix& src, qlm::Matrix& dst)
	{
		for (int r = 0; r < src.Rows(); r++)
		{
			for (int c = 0; c < src.Columns(); c++)
			{
				float res = src.Get(r, c);
				dst.Set(c, r, res);
			}
		}
	}
	///////////////////////////////////////////////////////////////////////////
	void VectorAdd_Row(const qlm::Matrix& src1, const qlm::Vector& src2, qlm::Matrix& dst)
	{
		for (int c = 0; c < src1.Columns(); c++)
		{
			for (int r = 0; r < src1.Rows(); r++)
			{
				float res = src1.Get(r, c) + src2.Get(r);
				dst.Set(r, c, res);
			}
		}
	}
	///////////////////////////////////////////////////////////////////////////
	void VectorDiv_Row(const qlm::Matrix& src1, const qlm::Vector& src2, qlm::Matrix& dst)
	{
		for (int c = 0; c < src1.Columns(); c++)
		{
			for (int r = 0; r < src1.Rows(); r++)
			{
				float res = src1.Get(r, c) / src2.Get(r);
				dst.Set(r, c, res);
			}
		}
	}
	///////////////////////////////////////////////////////////////////////////
	void VectorMul_Row(const qlm::Matrix& src1, const qlm::Vector& src2, qlm::Matrix& dst)
	{
		for (int c = 0; c < src1.Columns(); c++)
		{
			for (int r = 0; r < src1.Rows(); r++)
			{
				float res = src1.Get(r, c) * src2.Get(r);
				dst.Set(r, c, res);
			}
		}
	}
	///////////////////////////////////////////////////////////////////////////
	void VectorSub_Row(const qlm::Matrix& src1, const qlm::Vector& src2, qlm::Matrix& dst)
	{
		for (int c = 0; c < src1.Columns(); c++)
		{
			for (int r = 0; r < src1.Rows(); r++)
			{
				float res = src1.Get(r, c) - src2.Get(r);
				dst.Set(r, c, res);
			}
		}
	}
	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
}