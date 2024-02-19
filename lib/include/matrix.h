#pragma once

#include <limits>
#include "types.h"
#include "thread_pool.h"
namespace qlm
{
    class Vector;
    class Neuron;
    class Matrix {
    private:
        float* data;
        int columns;
        int rows;

    public:
        template<auto op, typename... Args>
        Status MatrixProc_ElemWise(Matrix& dst, ThreadPool& pool, Args... args) const;

        template<auto op, typename... Args>
        Status MatrixProc_ElemWise(const Matrix& src, Matrix& dst, ThreadPool& pool, Args... args) const;

        template<auto op, typename... Args>
        Status MatrixProc_ElemWise(const Vector& src, Matrix& dst, const BroadCast& broad_cast, ThreadPool& pool, Args... args) const;

        template<auto op, typename... Args>
        Status MatrixProc_Dot(const Matrix& src, Matrix& dst,ThreadPool& pool, Args... args) const;

        template<auto op, typename... Args>
        Status MatrixProc_Dot(const Vector& src, Vector& dst,ThreadPool& pool, Args... args) const;

    public:
        Matrix();
        Matrix(int r, int c);
        Matrix(const Matrix& other);
        ~Matrix();
        // move and = operator
    public:
        void Set(int row, int col, float value);
        void Set(int i, float value);
        float Get(int row, int col) const;
        float Get(int i) const;
        int Columns() const;
        int Rows() const;

    public:
        // print matrix
        void Print() const;
        // random initialization
        void RandomInit(const float min_value, const float max_value);
    public:
        // matrix-matrix operations
        // matrix addition
        Status Add(const Matrix& src, Matrix& dst, ThreadPool& pool) const;
        // matrix subtraction
        Status Sub(const Matrix& src, Matrix& dst, ThreadPool& pool) const;
        // matrix element wise multiplication
        Status Mul(const Matrix& src, Matrix& dst, ThreadPool& pool) const;
        // matrix element wise division
        Status Div(const Matrix& src, Matrix& dst, ThreadPool& pool) const;
        // matrix multiplication
        Status Dot(const Matrix& src, Matrix& dst, ThreadPool& pool) const;
        // matrix transpose
        Status Transpose(Matrix& dst, ThreadPool& pool) const;
    public:
        // matrix-vector operations
        // addition
        Status Add(const Vector& src, Matrix& dst, const BroadCast& broad_cast, ThreadPool& pool) const;
        // subtraction
        Status Sub(const Vector& src, Matrix& dst, const BroadCast& broad_cast, ThreadPool& pool) const;
        // multiplication
        Status Mul(const Vector& src, Matrix& dst, const BroadCast& broad_cast, ThreadPool& pool) const;
        // division
        Status Div(const Vector& src, Matrix& dst, const BroadCast& broad_cast, ThreadPool& pool) const;
        // dot
        Status Dot(const Vector& src, Vector& dst, ThreadPool& pool) const;
    public:
        // matrix-scalar operations
        // addition
        Status Add(const float src, Matrix& dst, ThreadPool& pool) const;
        // subtraction
        Status Sub(const float src, Matrix& dst, ThreadPool& pool) const;
        // element wise multiplication
        Status Mul(const float src, Matrix& dst, ThreadPool& pool) const;
        // element wise division
        Status Div(const float src, Matrix& dst, ThreadPool& pool) const;

        friend class Neuron;
    };
}