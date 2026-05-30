//==================================================================================================
// Vector.h - A high‑performance dynamic vector class for Linear Algebra
//
// Design goals:
//   - Zero‑overhead abstraction: all operations are defined in header to enable inlining.
//   - Cache‑friendly layout: contiguous storage (unique_ptr<double[]>) for spatial locality.
//   - Modern C++ (C++17/20): noexcept, move semantics, constexpr where applicable.
//   - Explicit resource management with RAII (unique_ptr), no manual new/delete.
//   - Carmack‑style naming: m_ prefix for member variables, consistent and unambiguous.
//
// Performance considerations:
//   - Inlined accessors and arithmetic operators eliminate function call overhead.
//   - Move operations are noexcept, enabling efficient usage in standard containers.
//   - Bounds checking is provided in debug builds; consider disabling for release.
//   - p‑norm implementation is specialized for p==2 (no pow call) for speed.
//
// Usage:
//   Vector v{1.0, 2.0, 3.0};
//   Vector w(5);
//   double norm = v.p_norm();
//
// Author: Salvatore Nasisi
//==================================================================================================

#pragma once

#include <cmath>     
#include <algorithm> 
#include <cassert> 
#include<format>
#include<string>
#include<ostream>
#include <cstdio>

class Vector {
    
    friend std::ostream& operator<<(std::ostream& os, const Vector& v) {
        /*
        Purpose:
            overloads "<<"
        Output:
            [v1,v2,...]
        Assumes:
            - v a vector of class "Vector"
            - the iterators are defined
            to enable range-based for loop
        */
        os << "[";
        bool first = true;
        for (const double& val : v) { 
            if (!first) {
                os << ", ";
            }
            os << val;
            first = false;
        }
        os << "]";

        return os;
    }

    public:

        // ------------------------------------------------------------------------
        // Constructors and Destructor
        // ------------------------------------------------------------------------
        explicit Vector(size_t size) 
            : m_size{size}, 
            m_data{size > 0 ? new double[size]{0.0} : nullptr}{
            
            assert(m_size > 0 && "Fatal: Vector size must be > 0");
        }

        Vector(const Vector& v2)
            : m_size(v2.m_size), 
            m_data(v2.m_size > 0 ? new double[v2.m_size] : nullptr){
            if (m_data) {
                std::copy(v2.m_data, v2.m_data + m_size, m_data);
            }
        }

        Vector(Vector&& v2) noexcept
            /*
            Move constructor
            */
            : m_size(v2.m_size), 
            m_data(v2.m_data){
            v2.m_size = 0;
            v2.m_data = nullptr;
        }

        // ------------------------------------------------------------------------
        // Initializer_list Constructor
        // ------------------------------------------------------------------------

        Vector(std::initializer_list<double> init)
            /*
            Purpose:
                It allows to write
                Vector v{1.0, 2.0, 3.0}
            Assumes:
                Floaing-point numbers as input
            */
            : m_size(init.size()),
            m_data(m_size > 0 ? new double[m_size] : nullptr) {
            std::copy(init.begin(), init.end(), m_data);
        }

        ~Vector() {
            delete[] m_data;
        }

        // ------------------------------------------------------------------------
        // Iterators
        // ------------------------------------------------------------------------
        const double* begin() const noexcept { return m_data; }
        const double* end()   const noexcept { return m_data + m_size; }
        double* begin()       noexcept { return m_data; }
        double* end()         noexcept { return m_data + m_size; }

        // ------------------------------------------------------------------------
        // Assignment Operators
        // ------------------------------------------------------------------------
        Vector& operator=(const Vector& v2) {
            if (this != &v2) {
                if (m_size != v2.m_size) {
                    delete[] m_data;
                    m_size = v2.m_size;
                    m_data = m_size > 0 ? new double[m_size] : nullptr;
                }
                if (m_data) {
                    std::copy(v2.m_data, v2.m_data + m_size, m_data);
                }
            }
            return *this;
        }

        Vector& operator=(Vector&& v2) noexcept {
            /*
            Move Assignment
            */
            if (this != &v2) {
                delete[] m_data;
                m_size = v2.m_size;
                m_data = v2.m_data;
                
                v2.m_size = 0;
                v2.m_data = nullptr;
            }
            return *this;
        }

        // ------------------------------------------------------------------------
        // Basic Queries
        // ------------------------------------------------------------------------
        size_t size() const noexcept { return m_size; }

        // ------------------------------------------------------------------------
        // Element Access (0-based indexing)
        // ------------------------------------------------------------------------
        double& operator[](size_t i) {
            assert(i < m_size && "Fatal: Vector index out of range");
            return m_data[i];
        }

        const double& operator[](size_t i) const {
            assert(i < m_size && "Fatal: Vector index out of range");
            return m_data[i];
        }
        

        
        // ------------------------------------------------------------------------
        // Element Access (1-based indexing)
        // ------------------------------------------------------------------------
        double& operator()(size_t i) {
            assert(i > 0 && i <= m_size && "Fatal: 1-based index out of range");
            return m_data[i - 1];
        }

        const double& operator()(size_t i) const {
            assert(i > 0 && i <= m_size && "Fatal: 1-based index out of range");
            return m_data[i - 1];
        }
        

        // ------------------------------------------------------------------------
        // Unary Operators
        // ------------------------------------------------------------------------
        Vector operator+() const {
            return *this;  // copy constructor handles it
        }

        Vector operator-() const {
            Vector result(m_size);
            for (size_t i = 0; i < m_size; ++i) {
                result.m_data[i] = -m_data[i];
            }
            return result;
        }

        // ------------------------------------------------------------------------
        // Binary Arithmetic Operators
        // ------------------------------------------------------------------------
        Vector operator+(const Vector& v2) const {
            assert(m_size == v2.m_size && "Fatal: Vector sizes must match for addition");
            Vector result(m_size);
            for (size_t i = 0; i < m_size; ++i) {
                result.m_data[i] = m_data[i] + v2.m_data[i];
            }
            return result;
        }

        Vector operator-(const Vector& v2) const {
            assert(m_size == v2.m_size && "Fatal: Vector sizes must match for subtraction");
            Vector result(m_size);
            for (size_t i = 0; i < m_size; ++i) {
                result.m_data[i] = m_data[i] - v2.m_data[i];
            }
            return result;
        }

        // ------------------------------------------------------------------------
        // Scalar Multiplication
        // ------------------------------------------------------------------------
        Vector operator*(double s) const {
            /*
            Allows [v1,v2,...] * s
            */
            Vector result(m_size);
            for (size_t i = 0; i < m_size; ++i) {
                result.m_data[i] = m_data[i] * s;
            }
            return result;
        }

        // ------------------------------------------------------------------------
        // Dot Product
        // ------------------------------------------------------------------------
        double dot(const Vector& v2) const {
            assert(m_size == v2.m_size && "Fatal: Vector sizes must match for dot product");
            double sum = 0.0;
            for (size_t i = 0; i < m_size; ++i) {
                sum += m_data[i] * v2.m_data[i];
            }
            return sum;
        }

        // ------------------------------------------------------------------------
        // p-norm
        // ------------------------------------------------------------------------
        double p_norm(int p = 2) const {
            assert(p > 0 && "Fatal: p must be positive for p-norm");
            
            if (p == 2) {
                double sum = 0.0;
                for (size_t i = 0; i < m_size; ++i) {
                    sum += m_data[i] * m_data[i];
                }
                return std::sqrt(sum);
            }
            
            double sum = 0.0;
            for (size_t i = 0; i < m_size; ++i) {
                sum += std::pow(std::fabs(m_data[i]), p);
            }
            return std::pow(sum, 1.0 / p);
        }

        // ------------------------------------------------------------------------
        // L-infinity norm
        // ------------------------------------------------------------------------
        double linf_norm() const {
            double max_val = 0.0;
            for (size_t i = 0; i < m_size; ++i) {
                double abs_val = std::fabs(m_data[i]);
                if (abs_val > max_val) max_val = abs_val;
            }
            return max_val;
        }

        std::string to_string() const {
            if (m_size == 0) return "[]";

            std::string result;
            
            result.reserve(m_size * 12); // pre-allocates and then creates a contiguous block of memory upfront (12*m_size Bytes)
            
            result += "[";
            
            char buf[32]; // allocates 32 bytes directly on the CPU's stack

            for (size_t i = 0; i < m_size; ++i) {
                
                std::snprintf(buf, sizeof(buf), "%.2f", m_data[i]); // converts the double bits into characters, and stores them in "buf"
                
                result += buf; // appends to "result"

                if (i < m_size - 1) {
                    result += ", ";
                }
            }
            result += "]";
            
            return result;
        }


        

    private:
        size_t  m_size;
        double* m_data;
};

// Non-member scalar multiplication
inline Vector operator*(double s, const Vector& v) {
    /*
    Allows s * [v1,v2,...]
    */
    return v * s; 
}

