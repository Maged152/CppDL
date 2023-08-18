#pragma once

#include <limits>
namespace qlm
{
    class Vector {
    private:
        float* data;
        int len;

    private:
        template<typename op>
        Status VectorElemWiseOp(const Vector& src, Vector& dst, float utilization = 0.5f);

        template<typename op>
        Status VectorElemOp(const float src, Vector& dst, float utilization = 0.5f);

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
        // Getter for lenght
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
        // vector-vector operations
        // vector addition
        Status Add(const Vector& src, Vector& dst, float utilization = 0.5f);
        // vector subtraction
        Status Sub(const Vector& src, Vector& dst, float utilization = 0.5f);
        // vector multiplication
        Status Mul(const Vector& src, Vector& dst, float utilization = 0.5f);
        // vector division
        Status Div(const Vector& src, Vector& dst, float utilization = 0.5f);
    public:
        // Vector-scalar operations
        // addition
        Status Add(const float src, Vector& dst, float utilization = 0.5f);
        // subtraction
        Status Sub(const float src, Vector& dst, float utilization = 0.5f);
        // multiplication
        Status Mul(const float src, Vector& dst, float utilization = 0.5f);
        // division
        Status Div(const float src, Vector& dst, float utilization = 0.5f);
    };
}