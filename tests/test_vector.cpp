#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../include/novasolve/doctest.h"
#include "../include/novasolve/Vector.h"


TEST_CASE("Vector: Core Lifecycle (Constructor/Copy/Move)") {
    Vector v1(3);
    v1[0] = 1.0; v1[1] = 2.0; v1[2] = 3.0;

    SUBCASE("Construction and Sizing") {
        CHECK(v1.size() == 3);
        CHECK(v1[0] == doctest::Approx(1.0));
    }

    SUBCASE("Copy Semantics (Deep Copy)") {
        Vector v2 = v1; 
        CHECK(v2.size() == v1.size());
        v2[0] = 99.0;
        // Verify deep copy: v1 should remain unchanged
        CHECK(v1[0] == doctest::Approx(1.0)); 
        CHECK(v2[0] == doctest::Approx(99.0));
    }

    SUBCASE("Move Semantics") {
        Vector v3 = std::move(v1);
        CHECK(v3.size() == 3);
        CHECK(v3[2] == doctest::Approx(3.0));
        // v1 is now in a valid but unspecified state (size 0 in our impl)
        CHECK(v1.size() == 0); 
    }
}

TEST_CASE("Vector: Mathematical Operations") {
    Vector v1(2);
    v1[0] = 1.0; v1[1] = 2.0;

    Vector v2(2);
    v2[0] = 3.0; v2[1] = 4.0;

    SUBCASE("Binary Addition") {
        Vector res = v1 + v2;
        CHECK(res[0] == doctest::Approx(4.0));
        CHECK(res[1] == doctest::Approx(6.0));
    }

    SUBCASE("Binary Subtraction") {
        Vector res = v1 - v2;
        CHECK(res[0] == doctest::Approx(-2.0));
        CHECK(res[1] == doctest::Approx(-2.0));
    }

    SUBCASE("Scalar Multiplication") {
        Vector res1 = v1 * 3.0;
        CHECK(res1[0] == doctest::Approx(3.0));
        CHECK(res1[1] == doctest::Approx(6.0));

        Vector res2 = 0.5 * v1;
        CHECK(res2[0] == doctest::Approx(0.5));
        CHECK(res2[1] == doctest::Approx(1.0));
    }

    SUBCASE("Unary Negation") {
        Vector res = -v1;
        CHECK(res[0] == doctest::Approx(-1.0));
        CHECK(res[1] == doctest::Approx(-2.0));
    }
}

TEST_CASE("Vector: Norms and Reductions") {
    Vector v(3);
    v[0] = 3.0; v[1] = -4.0; v[2] = 0.0;

    SUBCASE("L2 Norm (Euclidean)") {
        // sqrt(3^2 + 4^2) = 5
        CHECK(v.p_norm(2) == doctest::Approx(5.0));
    }

    SUBCASE("L1 Norm (Manhattan)") {
        // |3| + |-4| + |0| = 7
        CHECK(v.p_norm(1) == doctest::Approx(7.0));
    }
}