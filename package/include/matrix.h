#pragma once

#include <limits>
#include "types.h"
#include "thread_pool.h"
namespace qlm
{
    class Vector;

    class Matrix {
    private:
        float* data;
        int columns;
        int rows;

    private:
        template<typename op>
        Status MatrixElemWiseOp(const Matrix& src, Matrix& dst,ThreadPool& pool);

        template<typename op>
        Status MatrixElemOp(const float src, Matrix& dst,ThreadPool& pool);

        template<typename op>
        Status MatrixVectorOp(const Vector& src, Matrix& dst, const BroadCast& broad_cast,ThreadPool& pool);

    public:
        // Default constructor
        Matrix() : data(nullptr), columns(0), rows(0)
        {}
        // Parameterized constructor
        Matrix(int r, int c) : columns(c), rows(r) 
        {
            data = new float[columns * rows];
        }
        // Copy constructor
        Matrix(const Matrix& other) : columns(other.columns), rows(other.rows) 
        {
            data = new float[columns * rows];
            for (int i = 0; i < columns * rows; ++i) {
                data[i] = other.data[i];
            }
        }
        // Destructor
        ~Matrix() 
        {
            rows = columns = 0;
            if (data != nullptr)
                delete[] data;
        }
        // move and = operator
    public:
        // Setter for individual element
        void Set(int row, int col, float value) 
        {
            if (row >= 0 && row < rows && col >= 0 && col < columns)
            {
                data[row * columns + col] = value;
            }
        }
        void Set(int i, float value) {
            if (i >= 0 && i < columns * rows)
            {
                data[i] = value;
            }
        }
        // Getter for individual element
        float Get(int row, int col) const 
        {
            if (row >= 0 && row < rows && col >= 0 && col < columns)
            {
                return data[row * columns + col];
            }
            return std::numeric_limits<float>::signaling_NaN();
        }
        float Get(int i) const
        {
            if (i >= 0 && i < columns * rows)
            {
                return data[i];
            }
            return std::numeric_limits<float>::signaling_NaN();
        }
        // Getter for columns
        int Columns() const
        {
            return columns;
        }
        // Getter for rows
        int Rows() const
        {
            return rows;
        }
    public:
        // print matrix
        void Print();
        // random initialization
        void RandomInit(const float min_value, const float max_value);
    public:
        // matrix-matrix operations
        // matrix addition
        Status Add(const Matrix& src, Matrix& dst,ThreadPool& pool);
        // matrix subtraction
        Status Sub(const Matrix& src, Matrix& dst,ThreadPool& pool);
        // matrix element wise multiplication
        Status Mul(const Matrix& src, Matrix& dst,ThreadPool& pool);
        // matrix element wise division
        Status Div(const Matrix& src, Matrix& dst,ThreadPool& pool);
        // matrix multiplication
        Status Dot(const Matrix& src, Matrix& dst,ThreadPool& pool);
        // matrix transpose
        Status Transpose(Matrix& dst,ThreadPool& pool);
    public:
        // matrix-vector operations
        // addition
        Status Add(const Vector& src, Matrix& dst, const BroadCast& broad_cast,ThreadPool& pool);
        // subtraction
        Status Sub(const Vector& src, Matrix& dst, const BroadCast& broad_cast,ThreadPool& pool);
        // multiplication
        Status Mul(const Vector& src, Matrix& dst, const BroadCast& broad_cast,ThreadPool& pool);
        // division
        Status Div(const Vector& src, Matrix& dst, const BroadCast& broad_cast,ThreadPool& pool);
    public:
        // matrix-scalar operations
        // addition
        Status Add(const float src, Matrix& dst,ThreadPool& pool);
        // subtraction
        Status Sub(const float src, Matrix& dst,ThreadPool& pool);
        // element wise multiplication
        Status Mul(const float src, Matrix& dst,ThreadPool& pool);
        // element wise division
        Status Div(const float src, Matrix& dst,ThreadPool& pool);

    };
}