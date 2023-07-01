#pragma once

#include <limits>
namespace qlm
{
    class Matrix {
    private:
        float* data;
        int width;
        int height;

    public:
        // Default constructor
        Matrix() : data(nullptr), width(0), height(0)
        {}
        // Parameterized constructor
        Matrix(int w, int h) : width(w), height(h) 
        {
            data = new float[width * height];
        }
        // Copy constructor
        Matrix(const Matrix& other) : width(other.width), height(other.height) 
        {
            data = new float[width * height];
            for (int i = 0; i < width * height; ++i) {
                data[i] = other.data[i];
            }
        }
        // Destructor
        ~Matrix() 
        {
            if (data != nullptr)
                delete[] data;
        }
    public:
        // Setter for individual element
        void Set(int row, int col, float value) 
        {
            if (row >= 0 && row < height && col >= 0 && col < width) 
            {
                data[row * width + col] = value;
            }
        }

        void Set(int i, float value) {
            if (i >= 0 && i < width * height)
            {
                data[i] = value;
            }
        }
        // Getter for individual element
        float Get(int row, int col) const 
        {
            if (row >= 0 && row < height && col >= 0 && col < width) 
            {
                return data[row * width + col];
            }
            return std::numeric_limits<float>::signaling_NaN();
        }

        float Get(int i) const
        {
            if (i >= 0 && i < width * height)
            {
                return data[i];
            }
            return std::numeric_limits<float>::signaling_NaN();
        }
        // Getter for width
        int Width() const 
        {
            return width;
        }
        // Getter for height
        int Height() const
        {
            return height;
        }
        // random initialization

        // print matrix
        void Print()
        {

        }
    public:
        void MatrixAdd(const Matrix& src, Matrix& dst);
    };
}