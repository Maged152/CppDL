#pragma once

#include <limits>
#include "types.h"
#include "thread_pool.h"

namespace qlm
{
    class Matrix;

    class Vector {
    private:
        float* data;
        int len;

    public:
        template<float (*op)(const float, const float)>
        Status VectorProc_1Scalar_Out(float& dst, ThreadPool& pool) const;

        template<float (*op)(const float, const float, const float)>
        Status VectorProc_1Scalar_Out(const Vector& src, float& dst, ThreadPool& pool) const;

        template <void (*op)(const float, float&, float&)>
        Status VectorProc_2Scalar_Out(float& dst0, float& dst1, ThreadPool& pool) const;
        
        template <void (*op)(const float, const size_t, float&, size_t&)>
        Status VectorProc_1ArgScalar_Out(size_t& dst, ThreadPool& pool) const;

        template <void (*op)(const float, const size_t, float&, size_t&, float&, size_t&)>
        Status VectorProc_2ArgScalar_Out(size_t& dst0, size_t& dst1, ThreadPool& pool) const;

        template<float (*op)(const float)>
        Status VectorProc_ElemWise(Vector& dst, ThreadPool& pool) const;

        template<float (*op)(const float, const float)>
        Status VectorProc_ElemWise(const Vector& src, Vector& dst, ThreadPool& pool) const;

        template<float (*op)(const float, const float)>
        Status VectorProc_ElemWise(const float src, Vector& dst, ThreadPool& pool) const;

    public:
        // Default constructor
        Vector() : data(nullptr), len(0)
        {}
        // Parameterized constructor
        Vector(int l) : len(l)
        {
            data = new float[l];
        }
        // Copy constructor
        Vector(const Vector& other) : len(other.len)
        {
            data = new float[len];
            for (int i = 0; i < len; ++i) {
                data[i] = other.data[i];
            }
        }
        // Destructor
        ~Vector()
        {
            if (data != nullptr)
                delete[] data;
        }
    public:
        // Setter for individual element
        void Set(int i, float value) {
            if (i >= 0 && i < len)
            {
                data[i] = value;
            }
        }
        // Getter for individual element
        float Get(int i) const
        {
            if (i >= 0 && i <len)
            {
                return data[i];
            }
            return std::numeric_limits<float>::signaling_NaN();
        }
        // Getter for length
        int Length() const
        {
            return len;
        }
    public:
        // print matrix
        void Print() const;
        // random initialization
        void RandomInit(const float min_value, const float max_value);
    public:
        // vector operations
        // dot product
        Status Dot(const Vector& src, float& dst, ThreadPool& pool) const;
        // magnitude
        Status Mag(float& dst, ThreadPool& pool) const;
        // unit vector
        Status Unit(Vector& dst, ThreadPool& pool) const;
        // angle
        Status Angle(const Vector& src, float& dst, ThreadPool& pool) const;
        // sum
        Status Sum(float& dst, ThreadPool& pool) const;
        // mean
        Status Mean(float& dst, ThreadPool& pool) const;
        // variance
        Status Var(float& dst, ThreadPool& pool) const;
        // min 
        Status Min(float& dst, ThreadPool& pool) const;
        // max 
        Status Max(float& dst, ThreadPool& pool) const;
        // min max
        Status MinMax(float& dst_min, float& dst_max, ThreadPool& pool) const;
        // norm
        Status Norm(const Norm norm,float& dst, ThreadPool& pool) const;
        // arg min 
        Status ArgMin(size_t& dst, ThreadPool& pool) const;
        // arg max 
        Status ArgMax(size_t& dst, ThreadPool& pool) const;
        // arg min max
        Status ArgMinMax(size_t& dst_min, size_t& dst_max, ThreadPool& pool) const;

    public:
        // vector-vector operations
        // vector addition
        Status Add(const Vector& src, Vector& dst, ThreadPool& pool) const;
        // vector subtraction
        Status Sub(const Vector& src, Vector& dst, ThreadPool& pool) const;
        // vector multiplication
        Status Mul(const Vector& src, Vector& dst, ThreadPool& pool) const;
        // vector division
        Status Div(const Vector& src, Vector& dst, ThreadPool& pool) const;
        // covariance
        Status Cov(const Vector& src, float& dst, ThreadPool& pool) const;
        // correlation
        Status Corr(const Vector& src, float& dst, ThreadPool& pool) const;
    public:
        // Vector-scalar operations
        // addition
        Status Add(const float src, Vector& dst, ThreadPool& pool) const;
        // subtraction
        Status Sub(const float src, Vector& dst, ThreadPool& pool) const;
        // multiplication
        Status Mul(const float src, Vector& dst, ThreadPool& pool) const;
        // division
        Status Div(const float src, Vector& dst, ThreadPool& pool) const;

        //--------------------------------------------------------------//
        friend class Matrix;
    };
}