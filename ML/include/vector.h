#pragma once

#include <limits>
namespace qlm
{
    class Vector {
    private:
        float* data;
        int len;

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
        // random initialization
    public:
        void VectorAdd(const Vector& src, Vector& dst);
    };
}