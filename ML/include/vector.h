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
        template<typename op>
        Status VectorElemWiseOp(const Vector& src, Vector& dst, ThreadPool& pool);

        template<typename op>
        Status VectorElemOp(const float src, Vector& dst, ThreadPool& pool);

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
        void Print();
        // random initialization
        void RandomInit(const float min_value, const float max_value);
    public:
        // vector operations
        // dot product
        Status Dot(const Vector& src, float& dst, ThreadPool& pool);
        // magnitude
        Status Mag(float& dst, ThreadPool& pool);
        // unit vector
        Status Unit(Vector& dst, ThreadPool& pool);
        // angle
        Status Angle(const Vector& src, float& dst, ThreadPool& pool);
        // transform using transformation matrix
        Status Transform(const Matrix& transform, Vector& dst, ThreadPool& pool);
    public:
        // vector-vector operations
        // vector addition
        Status Add(const Vector& src, Vector& dst, ThreadPool& pool);
        // vector subtraction
        Status Sub(const Vector& src, Vector& dst, ThreadPool& pool);
        // vector multiplication
        Status Mul(const Vector& src, Vector& dst, ThreadPool& pool);
        // vector division
        Status Div(const Vector& src, Vector& dst, ThreadPool& pool);
    public:
        // Vector-scalar operations
        // addition
        Status Add(const float src, Vector& dst, ThreadPool& pool);
        // subtraction
        Status Sub(const float src, Vector& dst, ThreadPool& pool);
        // multiplication
        Status Mul(const float src, Vector& dst, ThreadPool& pool);
        // division
        Status Div(const float src, Vector& dst, ThreadPool& pool);

        //--------------------------------------------------------------//
        friend class Matrix;
    };
}