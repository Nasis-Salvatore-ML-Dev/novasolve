#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../include/novasolve/doctest.h"
#include "../include/novasolve/LU.h"
#include "../include/novasolve/MatrixGen.h"
#include <cmath>
#include <cstddef>

// ============================================================================
// TEST CASE 1: 3x3 hand-verified system, x = {1, 1, 1}
// ============================================================================

TEST_CASE("LU: 3x3 system with hand-verified solution x={1,1,1}") {
    // A * [1,1,1]^T = b
    Matrix A(3, 3);
    A(0,0)=2.0; A(0,1)=1.0; A(0,2)=1.0;
    A(1,0)=4.0; A(1,1)=3.0; A(1,2)=3.0;
    A(2,0)=8.0; A(2,1)=7.0; A(2,2)=9.0;
    Vector b(3);
    b[0]=4.0; b[1]=10.0; b[2]=24.0;

    Vector x = solve(A, b);

    CHECK(x[0] == doctest::Approx(1.0).epsilon(1e-12));
    CHECK(x[1] == doctest::Approx(1.0).epsilon(1e-12));
    CHECK(x[2] == doctest::Approx(1.0).epsilon(1e-12));
}

// ============================================================================
// TEST CASE 2: A[0][0] = 0, partial pivoting required, x = {1, 1, 1}
// ============================================================================

TEST_CASE("LU: system with zero A[0][0] requiring pivoting") {
    // A * [1,1,1]^T = b
    Matrix A(3, 3);
    A(0,0)=0.0; A(0,1)=2.0; A(0,2)=3.0;
    A(1,0)=4.0; A(1,1)=5.0; A(1,2)=6.0;
    A(2,0)=7.0; A(2,1)=8.0; A(2,2)=0.0;
    Vector b(3);
    b[0]=5.0; b[1]=15.0; b[2]=15.0;

    Vector x = solve(A, b);

    CHECK(x[0] == doctest::Approx(1.0).epsilon(1e-12));
    CHECK(x[1] == doctest::Approx(1.0).epsilon(1e-12));
    CHECK(x[2] == doctest::Approx(1.0).epsilon(1e-12));
}

// ============================================================================
// TEST CASE 3: 10x10 SPD system, residual ||Ax - b||_inf < 1e-10
// ============================================================================

TEST_CASE("LU: 10x10 SPD system, residual < 1e-10") {
    // make_spd constructs B^T*B + 10*I — well-conditioned by design
    Matrix A = MatrixGen::make_spd(10, false, -5.0, 5.0, 42);
    Vector b(10);
    for (std::size_t i = 0; i < 10; ++i)
        b[i] = static_cast<double>(i + 1);

    Vector x = solve(A, b);

    Vector r = A * x - b;
    CHECK(r.linf_norm() < 1e-10);
}

// ============================================================================
// TEST CASE 4: Near-singular system — result must be finite, no NaN or Inf
// ============================================================================

TEST_CASE("LU: near-singular system produces finite result") {
    // Rank-1 matrix with tiny diagonal perturbation — condition number ~1e10
    Matrix A(3, 3);
    A(0,0)=1.0; A(0,1)=2.0; A(0,2)=3.0;
    A(1,0)=2.0; A(1,1)=4.0; A(1,2)=6.0;
    A(2,0)=3.0; A(2,1)=6.0; A(2,2)=9.0;
    A(1,1) += 1e-10;
    A(2,2) += 1e-10;
    Vector b(3);
    b[0]=1.0; b[1]=1.0; b[2]=1.0;

    Vector x = solve(A, b);

    for (std::size_t i = 0; i < 3; ++i)
        CHECK(std::isfinite(x[i]));
}
