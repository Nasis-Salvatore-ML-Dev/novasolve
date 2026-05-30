#pragma once

#include <random>
#include <cassert>
#include <cstddef>
#include "Matrix.h"

// ============================================================================
// MatrixGen — random matrix generators for testing and experimentation.
// seed=0 is non-deterministic. Any fixed seed gives reproducible results.
// ============================================================================

namespace MatrixGen {

// -------------------------------------------------------------------------
// Internal helpers
// -------------------------------------------------------------------------

// Returns a seeded Mersenne Twister. seed=0 means non-deterministic.
inline std::mt19937 make_rng(unsigned int seed) {
    if (seed == 0) {
        std::random_device rd;
        return std::mt19937{rd()};
    }
    return std::mt19937{seed};
}

// Draws one value from rng — real in [lo, hi] or integer in [lo, hi]
inline double draw(std::mt19937& rng, double lo, double hi, bool integer_entries) {
    if (integer_entries) {
        std::uniform_int_distribution<int> dist(
            static_cast<int>(lo), static_cast<int>(hi));
        return static_cast<double>(dist(rng));
    }
    std::uniform_real_distribution<double> dist(lo, hi);
    return dist(rng);
}

// -------------------------------------------------------------------------
// make_random — general m×n matrix
// -------------------------------------------------------------------------
inline Matrix make_random(
    std::size_t     rows,
    std::size_t     cols,
    bool            integer_entries = false,
    double          lo              = -10.0,
    double          hi              =  10.0,
    unsigned int    seed            = 0)
{
    assert(rows > 0 && cols > 0 && "MatrixGen::make_random: dimensions must be > 0");
    auto rng = make_rng(seed);
    Matrix A(rows, cols);
    for (std::size_t i = 0; i < rows; ++i)
        for (std::size_t j = 0; j < cols; ++j)
            A(i,j) = draw(rng, lo, hi, integer_entries);
    return A;
}

// -------------------------------------------------------------------------
// make_symmetric — symmetric n×n matrix
//
// Fills upper triangle randomly, mirrors to lower triangle.
// Not guaranteed positive definite — use make_spd for CG.
// -------------------------------------------------------------------------
inline Matrix make_symmetric(
    std::size_t     n,
    bool            integer_entries = false,
    double          lo              = -10.0,
    double          hi              =  10.0,
    unsigned int    seed            = 0)
{
    assert(n > 0 && "MatrixGen::make_symmetric: dimension must be > 0");
    auto rng = make_rng(seed);
    Matrix A(n, n);
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = i; j < n; ++j) {
            double val = draw(rng, lo, hi, integer_entries);
            A(i,j) = val;
            A(j,i) = val;
        }
    }
    return A;
}

// -------------------------------------------------------------------------
// make_spd — symmetric positive definite n×n matrix
//
// Construction: A = B^T * B + n*I
//   B^T * B is symmetric and positive semidefinite by construction.
//   Adding n*I (diagonal shift) guarantees strict positive definiteness
//   and improves conditioning — safe for CG without preconditioning.
// -------------------------------------------------------------------------
inline Matrix make_spd(
    std::size_t     n,
    bool            integer_entries = false,
    double          lo              = -5.0,
    double          hi              =  5.0,
    unsigned int    seed            = 0)
{
    assert(n > 0 && "MatrixGen::make_spd: dimension must be > 0");

    // Generate random B, then compute A = B^T * B + n*I
    Matrix B = make_random(n, n, integer_entries, lo, hi, seed);
    Matrix A = B.transpose() * B;

    // Diagonal shift: guarantees positive definiteness
    for (std::size_t i = 0; i < n; ++i)
        A(i,i) += static_cast<double>(n);

    return A;
}

} // namespace MatrixGen