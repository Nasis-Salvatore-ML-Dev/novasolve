
#include <iostream>
#include <cmath>
#include <cassert>

#include "../include/novasolve/Vector.h"

// Helper function to compare floating point numbers
bool is_close(double a, double b, double epsilon = 1e-9) {
    return std::fabs(a - b) < epsilon;
}

void test_initialization() {
    Vector v(5);
    assert(v.size() == 5);
    for (size_t i = 0; i < v.size(); ++i) {
        assert(is_close(v[i], 0.0));
    }
    std::cout << "[OK] Initialization\n";
}

void test_element_access() {
    Vector v(3);
    v[0] = 1.5;
    v[1] = -2.0;
    v[2] = 3.14;

    assert(is_close(v[0], 1.5));
    assert(is_close(v[1], -2.0));
    assert(is_close(v[2], 3.14));
    std::cout << "[OK] Element Access\n";
}

void test_copy_semantics() {
    Vector v1(3);
    v1[0] = 1.0; v1[1] = 2.0; v1[2] = 3.0;

    Vector v2 = v1; // Copy constructor
    assert(v2.size() == v1.size());
    assert(is_close(v2[1], 2.0));

    v2[1] = 99.0;
    assert(is_close(v1[1], 2.0)); // Ensure deep copy (v1 is untouched)

    Vector v3(3);
    v3 = v1; // Assignment operator
    assert(is_close(v3[2], 3.0));
    std::cout << "[OK] Copy Semantics\n";
}

void test_vector_math() {
    Vector v1(2);
    v1[0] = 1.0; v1[1] = 2.0;

    Vector v2(2);
    v2[0] = 3.0; v2[1] = 4.0;

    // Addition
    Vector v3 = v1 + v2;
    assert(is_close(v3[0], 4.0));
    assert(is_close(v3[1], 6.0));

    // Subtraction
    Vector v4 = v1 - v2;
    assert(is_close(v4[0], -2.0));
    assert(is_close(v4[1], -2.0));

    // Scalar Multiplication
    Vector v5 = v1 * 5.0;
    assert(is_close(v5[0], 5.0));
    assert(is_close(v5[1], 10.0));

    Vector v6 = 2.0 * v1;
    assert(is_close(v6[1], 4.0));
    std::cout << "[OK] Vector Math\n";
}

void test_norms() {
    Vector v(3);
    v[0] = 3.0; v[1] = -4.0; v[2] = 0.0;

    // L2 Norm (Euclidean distance: sqrt(9 + 16) = 5)
    assert(is_close(v.p_norm(2), 5.0));
    
    // L1 Norm (Manhattan distance: |3| + |-4| + |0| = 7)
    assert(is_close(v.p_norm(1), 7.0));
    std::cout << "[OK] p-Norms\n";
}

int main() {
    std::cout << "--- Starting Vector Tests ---\n";
    
    test_initialization();
    test_element_access();
    test_copy_semantics();
    test_vector_math();
    test_norms();

    std::cout << "--- All Tests Passed Successfully! ---\n";
    return 0;
}