#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../include/novasolve/doctest.h"
#include "../include/novasolve/Matrix.h"

// ============================================================================
// Helpers
// ============================================================================

// [ 1  2  3 ]
// [ 2  4  5 ]
// [ 3  5  6 ]
static Matrix make_A() {
    Matrix A(3, 3);
    A(0,0)=1.0; A(0,1)=2.0; A(0,2)=3.0;
    A(1,0)=2.0; A(1,1)=4.0; A(1,2)=5.0;
    A(2,0)=3.0; A(2,1)=5.0; A(2,2)=6.0;
    return A;
}

// [ 2  3  4 ]
// [ 3  5  6 ]
// [ 4  6  7 ]
static Matrix make_B() {
    Matrix B(3, 3);
    B(0,0)=2.0; B(0,1)=3.0; B(0,2)=4.0;
    B(1,0)=3.0; B(1,1)=5.0; B(1,2)=6.0;
    B(2,0)=4.0; B(2,1)=6.0; B(2,2)=7.0;
    return B;
}

// ============================================================================
// TEST CASE 1: Construction
// ============================================================================

TEST_CASE("Matrix: Construction") {

    SUBCASE("Correct dimensions and size") {
        Matrix A(3, 4);
        CHECK(A.rows() == 3);
        CHECK(A.cols() == 4);
        CHECK(A.size() == 12);
    }

    SUBCASE("Zero initialisation") {
        Matrix A(3, 3);
        for (std::size_t i = 0; i < A.rows(); ++i)
            for (std::size_t j = 0; j < A.cols(); ++j)
                CHECK(A(i,j) == doctest::Approx(0.0));
    }

    SUBCASE("Element write and read back") {
        Matrix A = make_A();
        CHECK(A(0,0) == doctest::Approx(1.0));
        CHECK(A(1,1) == doctest::Approx(4.0));
        CHECK(A(2,2) == doctest::Approx(6.0));
        CHECK(A(0,2) == doctest::Approx(3.0));
        CHECK(A(2,0) == doctest::Approx(3.0));
    }

    SUBCASE("fill()") {
        Matrix A(3, 3);
        A.fill(7.0);
        for (std::size_t i = 0; i < A.rows(); ++i)
            for (std::size_t j = 0; j < A.cols(); ++j)
                CHECK(A(i,j) == doctest::Approx(7.0));
    }
}

// ============================================================================
// TEST CASE 2: Rule of Five
// ============================================================================

TEST_CASE("Matrix: Rule of Five") {

    SUBCASE("Copy constructor — deep copy") {
        Matrix A = make_A();
        Matrix B{A};
        CHECK(B(0,0) == doctest::Approx(1.0));
        CHECK(B(1,1) == doctest::Approx(4.0));
        B(0,0) = 99.0;
        CHECK(A(0,0) == doctest::Approx(1.0));  // A unaffected
    }

    SUBCASE("Move constructor — transfers ownership") {
        Matrix A = make_A();
        Matrix B{std::move(A)};
        CHECK(B(0,0) == doctest::Approx(1.0));
        CHECK(B(1,1) == doctest::Approx(4.0));
        CHECK(A.rows() == 0);
        CHECK(A.cols() == 0);
        CHECK(A.size() == 0);
        CHECK(A.data() == nullptr);
    }

    SUBCASE("Copy assignment — deep copy") {
        Matrix A = make_A();
        Matrix B(3, 3);
        B = A;
        CHECK(B(2,2) == doctest::Approx(6.0));
        B(2,2) = 99.0;
        CHECK(A(2,2) == doctest::Approx(6.0));  // A unaffected
    }

    SUBCASE("Copy assignment — reallocation on size mismatch") {
        Matrix A = make_A();
        Matrix B(2, 2);
        B = A;
        CHECK(B.rows() == 3);
        CHECK(B.cols() == 3);
        CHECK(B(1,2) == doctest::Approx(5.0));
    }

    SUBCASE("Move assignment — transfers ownership") {
        Matrix A = make_A();
        Matrix B(3, 3);
        B = std::move(A);
        CHECK(B(0,0) == doctest::Approx(1.0));
        CHECK(A.data() == nullptr);
    }

    SUBCASE("Self copy assignment — no crash, no corruption") {
        Matrix A = make_A();
        A = A;
        CHECK(A(0,0) == doctest::Approx(1.0));
        CHECK(A(1,1) == doctest::Approx(4.0));
    }

    SUBCASE("Self move assignment — no crash, no corruption") {
        Matrix A = make_A();
        A = std::move(A);
        CHECK(A(0,0) == doctest::Approx(1.0));
    }
}

// ============================================================================
// TEST CASE 3: Arithmetic
// ============================================================================

TEST_CASE("Matrix: Arithmetic") {

    SUBCASE("Unary negation") {
        Matrix A = make_A();
        Matrix R = -A;
        CHECK(R(0,0) == doctest::Approx(-1.0));
        CHECK(R(1,1) == doctest::Approx(-4.0));
        CHECK(R(2,2) == doctest::Approx(-6.0));
    }

    SUBCASE("Binary addition") {
        Matrix A = make_A();
        Matrix B = make_B();
        Matrix R = A + B;
        // (0,0): 1+2=3  (1,1): 4+5=9  (2,1): 5+6=11
        CHECK(R(0,0) == doctest::Approx(3.0));
        CHECK(R(1,1) == doctest::Approx(9.0));
        CHECK(R(2,1) == doctest::Approx(11.0));
    }

    SUBCASE("Binary subtraction") {
        Matrix A = make_A();
        Matrix B = make_B();
        Matrix R = A - B;
        // (0,0): 1-2=-1  (1,1): 4-5=-1  (2,1): 5-6=-1
        CHECK(R(0,0) == doctest::Approx(-1.0));
        CHECK(R(1,1) == doctest::Approx(-1.0));
        CHECK(R(2,1) == doctest::Approx(-1.0));
    }

    SUBCASE("Scalar multiplication — Matrix * scalar") {
        Matrix A = make_A();
        Matrix R = A * 3.0;
        CHECK(R(0,0) == doctest::Approx(3.0));
        CHECK(R(1,1) == doctest::Approx(12.0));
        CHECK(R(2,2) == doctest::Approx(18.0));
    }

    SUBCASE("Scalar multiplication — scalar * Matrix") {
        Matrix A = make_A();
        Matrix R = 2.0 * A;
        CHECK(R(0,0) == doctest::Approx(2.0));
        CHECK(R(1,2) == doctest::Approx(10.0));
        CHECK(R(2,2) == doctest::Approx(12.0));
    }

    SUBCASE("Matrix-matrix product") {
        // Hand-computed: A*B where A=make_A(), B=make_B()
        // C(0,0) = 1*2 + 2*3 + 3*4 = 2+6+12 = 20
        // C(1,1) = 2*3 + 4*5 + 5*6 = 6+20+30 = 56
        // C(2,2) = 3*4 + 5*6 + 6*7 = 12+30+42 = 84
        Matrix A = make_A();
        Matrix B = make_B();
        Matrix C = A * B;
        CHECK(C.rows() == 3);
        CHECK(C.cols() == 3);
        CHECK(C(0,0) == doctest::Approx(20.0));
        CHECK(C(1,1) == doctest::Approx(56.0));
        CHECK(C(2,2) == doctest::Approx(84.0));
    }

    SUBCASE("Matrix-vector product") {
        // A*v where v = [1, 0, 0] -> first column of A
        Matrix A = make_A();
        Vector v(3);
        v[0]=1.0; v[1]=0.0; v[2]=0.0;
        Vector w = A * v;
        CHECK(w[0] == doctest::Approx(1.0));
        CHECK(w[1] == doctest::Approx(2.0));
        CHECK(w[2] == doctest::Approx(3.0));
    }

    SUBCASE("Matrix-vector product — non-trivial") {
        // A*v where v = [1, 1, 1] -> row sums of A
        Matrix A = make_A();
        Vector v(3);
        v[0]=1.0; v[1]=1.0; v[2]=1.0;
        Vector w = A * v;
        CHECK(w[0] == doctest::Approx(6.0));   // 1+2+3
        CHECK(w[1] == doctest::Approx(11.0));  // 2+4+5
        CHECK(w[2] == doctest::Approx(14.0));  // 3+5+6
    }

    SUBCASE("Vector-Matrix product - trivial") {
        // A*v where v = [1, 0, 0] -> first column of A
        Matrix A = make_A();
        Vector v(3);
        v[0]=1.0; v[1]=0.0; v[2]=0.0;
        Vector w = v*A;
        CHECK(w[0] == doctest::Approx(1.0));
        CHECK(w[1] == doctest::Approx(2.0));
        CHECK(w[2] == doctest::Approx(3.0));
    }
    
    SUBCASE("Vector-Matrix product - non-trivial") {
        // A*v where v = [1, 0, 0] -> first column of A
        Matrix A = make_A();
        Vector v(3);
        v[0]=1.0; v[1]=1.0; v[2]=1.0;
        Vector w = v*A;
        CHECK(w[0] == doctest::Approx(6.0));
        CHECK(w[1] == doctest::Approx(11.0));
        CHECK(w[2] == doctest::Approx(14.0));
    }
}

// ============================================================================
// TEST CASE 4: Transpose
// ============================================================================

TEST_CASE("Matrix: Transpose") {

    SUBCASE("Square symmetric matrix — transpose equals original") {
        Matrix A = make_A();  // A is symmetric
        Matrix T = A.transpose();
        for (std::size_t i = 0; i < 3; ++i)
            for (std::size_t j = 0; j < 3; ++j)
                CHECK(T(i,j) == doctest::Approx(A(i,j)));
    }

    SUBCASE("Non-square matrix — dimensions swap and values correct") {
        // A: 2x3
        // [ 1  2  3 ]
        // [ 4  5  6 ]
        // T: 3x2
        // [ 1  4 ]
        // [ 2  5 ]
        // [ 3  6 ]
        Matrix A(2, 3);
        A(0,0)=1.0; A(0,1)=2.0; A(0,2)=3.0;
        A(1,0)=4.0; A(1,1)=5.0; A(1,2)=6.0;
        Matrix T = A.transpose();
        CHECK(T.rows() == 3);
        CHECK(T.cols() == 2);
        CHECK(T(0,0) == doctest::Approx(1.0));
        CHECK(T(1,0) == doctest::Approx(2.0));
        CHECK(T(2,0) == doctest::Approx(3.0));
        CHECK(T(0,1) == doctest::Approx(4.0));
        CHECK(T(1,1) == doctest::Approx(5.0));
        CHECK(T(2,1) == doctest::Approx(6.0));
    }
}

// ============================================================================
// TEST CASE 5: Norms
// ============================================================================

TEST_CASE("Matrix: Norms") {

    SUBCASE("Frobenius norm") {
        // [[3, 0], [0, 4]] -> sqrt(9+16) = 5
        Matrix A(2, 2);
        A(0,0)=3.0; A(0,1)=0.0;
        A(1,0)=0.0; A(1,1)=4.0;
        CHECK(A.norm_frobenius() == doctest::Approx(5.0));
    }

    SUBCASE("Infinity norm — max absolute row sum") {
        // [[ 1, -2], [-3,  4]]
        // row 0: |1|+|-2| = 3
        // row 1: |-3|+|4| = 7  <- max
        Matrix A(2, 2);
        A(0,0)=1.0;  A(0,1)=-2.0;
        A(1,0)=-3.0; A(1,1)=4.0;
        CHECK(A.norm_inf() == doctest::Approx(7.0));
    }
}