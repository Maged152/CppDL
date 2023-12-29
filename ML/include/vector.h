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

    private:
        // naming for common precessing function
        // Vector_input_lambda_output -> Vector_vv_vv_v
        template<typename op>
        Status VectorElemWiseOp(const Vector& src, Vector& dst, ThreadPool& pool) const;

        template<typename op>
        Status VectorElemOp(const float src, Vector& dst, ThreadPool& pool) const;

        template<float (*op)(const float, const float)>
        Status VectorProc_1Scalar_Out(float& dst, ThreadPool& pool) const;

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
        Status Norm(const NORM norm,float& dst, ThreadPool& pool) const;

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