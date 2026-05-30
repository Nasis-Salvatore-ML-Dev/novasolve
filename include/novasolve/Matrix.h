#pragma once

#include <algorithm>   // std::copy, std::fill
#include <cassert>
#include <cmath>       // std::sqrt
#include <cstddef>     // std::size_t
#include <iostream>    // std::ostream
#include <iomanip>     // std::setw, std::setprecision

#include "Vector.h"

// ============================================================================
// Matrix — flat contiguous row-major storage
//
// Storage layout: m_data[i*m_cols + j]
// One heap allocation per matrix instance.
// Cache-friendly: traversing a row is a sequential scan.
//
// Design notes:
//   - Rule of Five: destructor, copy ctor, move ctor, copy =, move =
//   - All moves are noexcept — enables STL container optimizations
//   - [[nodiscard]] on queries — silences accidental discards
//   - operator()(i,j) — 0-based, both const and non-const overloads
//   - No raw new/delete outside this class
// ============================================================================

class Matrix {

    // scalar * matrix — defined as free friend so both operand orders work
    friend Matrix operator*(double s, const Matrix& A) {
        return A * s;
    }

    friend Vector operator*(const Vector& v,const Matrix& A){
        assert((v.size() == A.m_rows) &&
            "operator*: Vector-Matrix inner dimensions must match");

        Vector w(A.m_cols); // represents an 1xn vector
        
        for (size_t j{0}; j < A.m_cols; ++j){
            
            double sum{0.0};

            for (size_t i{0}; i < A.m_rows; ++i){

                sum+=v[i] * A.m_data[i*A.m_cols+j];
            
            }

            w[j]=sum;
            
        }

        return w;

    }

public:

    // -------------------------------------------------------------------------
    // Construction & Destruction
    // -------------------------------------------------------------------------
    explicit Matrix(std::size_t rows, std::size_t cols)
        : m_rows{rows}
        , m_cols{cols}
        , m_size{rows * cols}
        , m_data{m_size > 0 ? new double[m_size]{} : nullptr}{

    }

    ~Matrix() {
        delete[] m_data;
    }

    // -------------------------------------------------------------------------
    // Copy constructor — deep copy, independent allocation
    // -------------------------------------------------------------------------
    Matrix(const Matrix& other)
        : m_rows{other.m_rows}
        , m_cols{other.m_cols}
        , m_size{other.m_size}
        , m_data{m_size > 0 ? new double[m_size] : nullptr}{
        std::copy(other.m_data, other.m_data + m_size, m_data);
    }

    // -------------------------------------------------------------------------
    // Move constructor — steal, don't copy
    // -------------------------------------------------------------------------
    Matrix(Matrix&& other) noexcept
        : m_rows{other.m_rows}
        , m_cols{other.m_cols}
        , m_size{other.m_size}
        , m_data{other.m_data}{
        other.m_rows = 0;
        other.m_cols = 0;
        other.m_size = 0;
        other.m_data = nullptr;
    }

    // -------------------------------------------------------------------------
    // Copy assignment
    // -------------------------------------------------------------------------
    Matrix& operator=(const Matrix& other) {
        if (this != &other) {
            if (m_size != other.m_size) {
                delete[] m_data;
                m_size = other.m_size;
                m_data = m_size > 0 ? new double[m_size] : nullptr;
            }
            m_rows = other.m_rows;
            m_cols = other.m_cols;
            std::copy(other.m_data, other.m_data + m_size, m_data);
        }
        return *this;
    }

    // -------------------------------------------------------------------------
    // Move assignment
    // -------------------------------------------------------------------------
    Matrix& operator=(Matrix&& other) noexcept {
        if (this != &other) {
            delete[] m_data;
            m_rows = other.m_rows;
            m_cols = other.m_cols;
            m_size = other.m_size;
            m_data = other.m_data;
            other.m_rows = 0;
            other.m_cols = 0;
            other.m_size = 0;
            other.m_data = nullptr;
        }
        return *this;
    }

    // -------------------------------------------------------------------------
    // Queries
    // -------------------------------------------------------------------------
    [[nodiscard]] std::size_t rows() const noexcept { return m_rows; }
    [[nodiscard]] std::size_t cols() const noexcept { return m_cols; }
    [[nodiscard]] std::size_t size() const noexcept { return m_size; }

    // -------------------------------------------------------------------------
    // Element access — 0-based, row-major: m_data[i*m_cols + j]
    // -------------------------------------------------------------------------
    double& operator()(std::size_t i, std::size_t j) {
        assert((i < m_rows && j < m_cols) && "Matrix: index out of range");
        return m_data[i * m_cols + j];
    }

    const double& operator()(std::size_t i, std::size_t j) const {
        assert((i < m_rows && j < m_cols) && "Matrix: index out of range");
        return m_data[i * m_cols + j];
    }

    // -------------------------------------------------------------------------
    // Raw data access — for BLAS interop and low-level kernels
    // -------------------------------------------------------------------------
    [[nodiscard]] double*       data()       noexcept { return m_data; }
    [[nodiscard]] const double* data() const noexcept { return m_data; }

    // -------------------------------------------------------------------------
    // Fill
    // -------------------------------------------------------------------------
    void fill(double val) noexcept {
        std::fill(m_data, m_data + m_size, val);
    }

    // -------------------------------------------------------------------------
    // Unary negation
    // -------------------------------------------------------------------------
    Matrix operator-() const {
        Matrix result(m_rows, m_cols);
        for (std::size_t k = 0; k < m_size; ++k)
            result.m_data[k] = -m_data[k];
        return result;
    }

    // -------------------------------------------------------------------------
    // Binary arithmetic — matrix + matrix, matrix - matrix
    // -------------------------------------------------------------------------
    Matrix operator+(const Matrix& B) const {
        assert((m_rows == B.m_rows && m_cols == B.m_cols) &&
               "Matrix::operator+: size mismatch");
        Matrix result(m_rows, m_cols);
        for (std::size_t k = 0; k < m_size; ++k)
            result.m_data[k] = m_data[k] + B.m_data[k];
        return result;
    }

    Matrix operator-(const Matrix& B) const {
        assert((m_rows == B.m_rows && m_cols == B.m_cols) &&
               "Matrix::operator-: size mismatch");
        Matrix result(m_rows, m_cols);
        for (std::size_t k = 0; k < m_size; ++k)
            result.m_data[k] = m_data[k] - B.m_data[k];
        return result;
    }

    // -------------------------------------------------------------------------
    // Scalar multiplication — matrix * scalar
    // -------------------------------------------------------------------------
    Matrix operator*(double s) const {
        Matrix result(m_rows, m_cols);
        for (std::size_t k = 0; k < m_size; ++k)
            result.m_data[k] = m_data[k] * s;
        return result;
    }

    
    // -------------------------------------------------------------------------
    // Matrix-vector product — w = A*v
    // -------------------------------------------------------------------------
    Vector operator*(const Vector& v) const {
        assert((m_cols == v.size()) &&
               "Matrix::operator*: Matrix-Vector inner dimensions must match");
        Vector w(m_rows);
        for (std::size_t i = 0; i < m_rows; ++i)
            for (std::size_t j = 0; j < m_cols; ++j)
                w[i] += m_data[i * m_cols + j] * v[j];
        return w;
    }

    // -------------------------------------------------------------------------
    // Matrix-matrix product — C = A*B
    // A: m x p,  B: p x n,  C: m x n
    // -------------------------------------------------------------------------
    Matrix operator*(const Matrix& B) const {
        assert((m_cols == B.m_rows) &&
               "Matrix::operator*: inner dimensions must match");
        const std::size_t m = m_rows;
        const std::size_t p = m_cols;
        const std::size_t n = B.m_cols;
        Matrix C(m, n);
        for (std::size_t i = 0; i < m; ++i)
            for (std::size_t j = 0; j < n; ++j)
                for (std::size_t k = 0; k < p; ++k)
                    C.m_data[i * n + j] += m_data[i * p + k] * B.m_data[k * n + j];
        return C;
    }

    // -------------------------------------------------------------------------
    // Transpose
    // -------------------------------------------------------------------------
    [[nodiscard]] Matrix transpose() const {
        Matrix T(m_cols, m_rows);
        for (std::size_t i = 0; i < m_rows; ++i)
            for (std::size_t j = 0; j < m_cols; ++j)
                T.m_data[j * m_rows + i] = m_data[i * m_cols + j];
        return T;
    }

    // -------------------------------------------------------------------------
    // Frobenius norm — sqrt(sum of squares of all elements)
    // -------------------------------------------------------------------------
    [[nodiscard]] double norm_frobenius() const noexcept {
        double sum = 0.0;
        for (std::size_t k = 0; k < m_size; ++k)
            sum += m_data[k] * m_data[k];
        return std::sqrt(sum);
    }

    // -------------------------------------------------------------------------
    // Infinity norm — max absolute row sum
    // -------------------------------------------------------------------------
    [[nodiscard]] double norm_inf() const noexcept {
        double max_row = 0.0;
        for (std::size_t i = 0; i < m_rows; ++i) {
            double row_sum = 0.0;
            for (std::size_t j = 0; j < m_cols; ++j)
                row_sum += std::fabs(m_data[i * m_cols + j]);
            if (row_sum > max_row)
                max_row = row_sum;
        }
        return max_row;
    }

    // -------------------------------------------------------------------------
    // Stream output
    // -------------------------------------------------------------------------
    friend std::ostream& operator<<(std::ostream& output, const Matrix& A) {
        output << std::fixed << std::setprecision(6);
        for (std::size_t i = 0; i < A.m_rows; ++i) {
            output << "[ ";
            for (std::size_t j = 0; j < A.m_cols; ++j) {
                output << std::setw(12) << A(i, j);
                if (j + 1 < A.m_cols) output << "  ";
            }
            output << " ]\n";
        }
        return output;
    }

private:
    std::size_t m_rows;
    std::size_t m_cols;
    std::size_t m_size;   // m_rows * m_cols — avoids recomputing in hot paths
    double*     m_data;   // flat row-major storage: element (i,j) at [i*m_cols+j]
};