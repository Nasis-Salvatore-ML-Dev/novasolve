// LU.h
// In-place LU factorization with partial pivoting (PA = LU).
// L and U are stored in a single matrix: strictly lower triangle holds
// the L multipliers, upper triangle (including diagonal) holds U.
// The diagonal of L is implicitly 1 and never stored.

#pragma once
#include "Matrix.h"
#include "Vector.h"
#include <vector>
#include <cstddef>
#include <cmath>
#include <algorithm>

// lu_factorize: in-place LU with partial pivoting (PA = LU).
// After return, A holds L in its strict lower triangle and U on and above
// the diagonal. P[i] = original row index now at row i of the permuted
// matrix (so y = Pb means y[i] = b[P[i]]).
inline void lu_factorize(Matrix& A, std::vector<std::size_t>& P) {
    const std::size_t n = A.rows();
    P.resize(n);
    for (std::size_t i = 0; i < n; ++i)
        P[i] = i;

    for (std::size_t k = 0; k < n - 1; ++k) {
        // Partial pivot: row with maximum |A(i,k)| for i >= k
        std::size_t pivot = k;
        double max_val = std::fabs(A(k, k));
        for (std::size_t i = k + 1; i < n; ++i) {
            const double val = std::fabs(A(i, k));
            if (val > max_val) { max_val = val; pivot = i; }
        }
        if (pivot != k) {
            for (std::size_t j = 0; j < n; ++j)
                std::swap(A(k, j), A(pivot, j));
            std::swap(P[k], P[pivot]);
        }
        // Gaussian elimination: store multipliers in lower triangle
        for (std::size_t i = k + 1; i < n; ++i) {
            A(i, k) /= A(k, k);
            for (std::size_t j = k + 1; j < n; ++j)
                A(i, j) -= A(i, k) * A(k, j);
        }
    }
}

// lu_solve: forward substitution (Ly = Pb) then back substitution (Ux = y).
// LU holds the factored matrix from lu_factorize, P the permutation vector.
inline Vector lu_solve(const Matrix& LU, const std::vector<std::size_t>& P,
                       const Vector& b) {
    const std::size_t n = LU.rows();

    // Apply permutation: y[i] = b[P[i]]
    Vector x(n);
    for (std::size_t i = 0; i < n; ++i)
        x[i] = b[P[i]];

    // Forward substitution: Ly = Pb (L has implicit unit diagonal)
    for (std::size_t i = 1; i < n; ++i)
        for (std::size_t j = 0; j < i; ++j)
            x[i] -= LU(i, j) * x[j];

    // Back substitution: Ux = y
    for (std::size_t i = n; i-- > 0; ) {
        for (std::size_t j = i + 1; j < n; ++j)
            x[i] -= LU(i, j) * x[j];
        x[i] /= LU(i, i);
    }

    return x;
}

// solve: convenience wrapper — copies A, factorizes in-place, then solves.
inline Vector solve(Matrix A, const Vector& b) {
    std::vector<std::size_t> P;
    lu_factorize(A, P);
    return lu_solve(A, P, b);
}
