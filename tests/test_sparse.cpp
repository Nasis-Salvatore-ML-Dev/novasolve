#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../include/novasolve/doctest.h"
#include "../include/novasolve/SparseMatrix.h"
#include "../include/novasolve/Matrix.h"
#include <cmath>
#include <cstddef>
#include <vector>

// ============================================================================
// TEST 1: 3x3 sparse — SpMV matches dense Matrix*Vector to 1e-12
// ============================================================================

TEST_CASE("SpMV: 3x3 sparse matches dense Matrix*Vector to 1e-12") {
    //  A = [ 4  0  2 ]
    //      [ 0  3  0 ]
    //      [ 1  0  5 ]
    std::vector<std::size_t> ri = {0, 0, 1, 2, 2};
    std::vector<std::size_t> ci = {0, 2, 1, 0, 2};
    std::vector<double>      v  = {4.0, 2.0, 3.0, 1.0, 5.0};
    SparseMatrix S(3, 3, ri, ci, v);

    Matrix A(3, 3);
    A(0, 0) = 4.0;  A(0, 2) = 2.0;
    A(1, 1) = 3.0;
    A(2, 0) = 1.0;  A(2, 2) = 5.0;

    Vector x{1.0, 2.0, 3.0};
    Vector ys = S.matvec(x);
    Vector yd = A * x;

    for (std::size_t i = 0; i < 3; ++i)
        CHECK(std::fabs(ys[i] - yd[i]) < 1e-12);
}

// ============================================================================
// TEST 2: 1000x1000 matrix with <1% fill — verify nnz() << n*n
// ============================================================================

TEST_CASE("SpMV: 1000x1000 diagonal, nnz < 1% of n*n") {
    const std::size_t n = 1000;
    std::vector<std::size_t> ri(n), ci(n);
    std::vector<double>      v(n);
    for (std::size_t i = 0; i < n; ++i) {
        ri[i] = i;
        ci[i] = i;
        v[i]  = static_cast<double>(i + 1);
    }
    SparseMatrix S(n, n, ri, ci, v);

    CHECK(S.rows() == n);
    CHECK(S.cols() == n);
    CHECK(S.nnz()  == n);
    CHECK(S.nnz()  <  n * n / 100);   // 1000 < 10000 — well under 1% fill
}

// ============================================================================
// TEST 3: Row with all zeros — SpMV still correct
// ============================================================================

TEST_CASE("SpMV: all-zero row produces zero in output") {
    //  A = [ 1  0  0 ]
    //      [ 0  0  0 ]   <- empty row
    //      [ 0  0  1 ]
    std::vector<std::size_t> ri = {0, 2};
    std::vector<std::size_t> ci = {0, 2};
    std::vector<double>      v  = {1.0, 1.0};
    SparseMatrix S(3, 3, ri, ci, v);

    Vector x{3.0, 7.0, 5.0};
    Vector y = S.matvec(x);

    CHECK(y[0] == doctest::Approx(3.0).epsilon(1e-12));
    CHECK(y[1] == doctest::Approx(0.0).epsilon(1e-12));
    CHECK(y[2] == doctest::Approx(5.0).epsilon(1e-12));
}

// ============================================================================
// TEST 4: Diagonal matrix — single non-zero per row
// ============================================================================

TEST_CASE("SpMV: diagonal matrix — one non-zero per row") {
    // diag(2, 3, 5, 7) * [1, 2, 4, 8] = [2, 6, 20, 56]
    std::vector<std::size_t> ri = {0, 1, 2, 3};
    std::vector<std::size_t> ci = {0, 1, 2, 3};
    std::vector<double>      v  = {2.0, 3.0, 5.0, 7.0};
    SparseMatrix S(4, 4, ri, ci, v);

    Vector x{1.0, 2.0, 4.0, 8.0};
    Vector y = S.matvec(x);

    CHECK(y[0] == doctest::Approx(2.0).epsilon(1e-12));
    CHECK(y[1] == doctest::Approx(6.0).epsilon(1e-12));
    CHECK(y[2] == doctest::Approx(20.0).epsilon(1e-12));
    CHECK(y[3] == doctest::Approx(56.0).epsilon(1e-12));
}
