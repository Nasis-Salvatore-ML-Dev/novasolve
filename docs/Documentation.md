# NovaSolve — Library Documentation

---

## Table of Contents

1. [Vector](#vector)
2. [Matrix](#matrix)
3. [MatrixGen](#matrixgen)
4. [LU Factorization](#lu-factorization)
5. [SparseMatrix](#sparsematrix)

---

---

# Vector

A dynamic, heap-allocated, cache-friendly vector of `double` values.  
Header: `include/Vector.h`

---

## Basic Examples

```cpp
#include "Vector.h"
#include <iostream>

int main() {
    // Construct from size (zero-initialized)
    Vector v(3);
    v[0] = 1.0; v[1] = 2.0; v[2] = 3.0;

    // Construct from initializer list
    Vector w{4.0, 5.0, 6.0};

    // Print
    std::cout << v << "\n";   // [1, 2, 3]
    std::cout << w << "\n";   // [4, 5, 6]
}
```

---

## Options

### Construction

| Syntax                       | Description                              |
| ---------------------------- | ---------------------------------------- |
| `Vector v(n)`                | Zero-initialized vector of size `n`      |
| `Vector v{1.0, 2.0, 3.0}`    | Initializer-list construction            |
| `Vector v(other)`            | Deep copy                                |
| `Vector v(std::move(other))` | Move — steals data, leaves `other` empty |

### Element Access

```cpp
Vector v{10.0, 20.0, 30.0};

v[0];       // 0-based: returns 10.0
v(1);       // 1-based: returns 10.0
v[1] = 99;  // assignment via 0-based index
```

### Arithmetic

```cpp
Vector a{1.0, 2.0, 3.0};
Vector b{4.0, 5.0, 6.0};

Vector c = a + b;       // [5, 7, 9]
Vector d = a - b;       // [-3, -3, -3]
Vector e = 2.0 * a;     // [2, 4, 6]
Vector f = a * 3.0;     // [3, 6, 9]
Vector g = -a;          // [-1, -2, -3]
```

### Dot Product

```cpp
Vector a{1.0, 2.0, 3.0};
Vector b{4.0, 5.0, 6.0};
double d = a.dot(b);    // 1*4 + 2*5 + 3*6 = 32.0
```

### Norms

```cpp
Vector v{3.0, 4.0};

double l2  = v.p_norm();    // L2: sqrt(9+16) = 5.0
double l1  = v.p_norm(1);   // L1: 3+4 = 7.0
double inf = v.linf_norm(); // L-inf: max(3,4) = 4.0
```

### Queries

```cpp
Vector v(5);
v.size();   // returns 5
```

### String Conversion

```cpp
Vector v{1.0, 2.5, 3.75};
std::string s = v.to_string();  // "[1.00, 2.50, 3.75]"
```

### Iterators

```cpp
Vector v{1.0, 2.0, 3.0};
for (double x : v)
    std::cout << x << "\n";
```

---

## Applications

```cpp
// Compute residual r = b - A*x
#include "Matrix.h"
#include "Vector.h"
#include <iostream>

int main() {
    Matrix A(3, 3);
    A(0,0)=2; A(0,1)=1; A(0,2)=1;
    A(1,0)=4; A(1,1)=3; A(1,2)=3;
    A(2,0)=8; A(2,1)=7; A(2,2)=9;

    Vector x{1.0, 1.0, 1.0};
    Vector b{4.0, 10.0, 26.0};

    Vector r = b - A * x;
    std::cout << "Residual: " << r.linf_norm() << "\n";
}
```

---

---

# Matrix

A dense, flat, row-major matrix of `double` values.  
Header: `include/Matrix.h`

---

## Basic Examples

```cpp
#include "Matrix.h"
#include <iostream>

int main() {
    Matrix A(3, 3);
    A(0,0)=2; A(0,1)=1; A(0,2)=1;
    A(1,0)=4; A(1,1)=3; A(1,2)=3;
    A(2,0)=8; A(2,1)=7; A(2,2)=9;

    std::cout << A << "\n";
}
```

---

## Options

### Construction

| Syntax                       | Description                 |
| ---------------------------- | --------------------------- |
| `Matrix A(m, n)`             | Zero-initialized m×n matrix |
| `Matrix A(other)`            | Deep copy                   |
| `Matrix A(std::move(other))` | Move — steals data          |

### Element Access

```cpp
Matrix A(2, 2);
A(0,0) = 1.0;
A(0,1) = 2.0;
double val = A(1,0);
```

### Fill

```cpp
Matrix A(3, 3);
A.fill(0.0);    // all zeros
A.fill(1.0);    // all ones
```

### Arithmetic

```cpp
Matrix A(2,2), B(2,2);

Matrix C = A + B;       // element-wise addition
Matrix D = A - B;       // element-wise subtraction
Matrix E = A * 2.0;     // scalar multiplication
Matrix F = 3.0 * A;     // scalar multiplication (reverse order)
Matrix G = -A;          // negation
Matrix H = A * B;       // matrix-matrix product
```

### Matrix-Vector Product

```cpp
Matrix A(3, 3);
Vector v{1.0, 2.0, 3.0};
Vector w = A * v;       // w = A*v
```

### Vector-Matrix Product

```cpp
Vector v{1.0, 2.0, 3.0};
Matrix A(3, 3);
Vector w = v * A;       // w = v^T * A
```

### Transpose

```cpp
Matrix A(2, 3);
Matrix T = A.transpose();   // T is 3x2
```

### Norms

```cpp
Matrix A(3, 3);
double frob = A.norm_frobenius();  // sqrt(sum of squares)
double inf  = A.norm_inf();        // max absolute row sum
```

### Queries

```cpp
Matrix A(3, 4);
A.rows();   // 3
A.cols();   // 4
A.size();   // 12
```

### Raw Data Access

```cpp
Matrix A(3, 3);
double* ptr = A.data();
// element (i,j) is at ptr[i * A.cols() + j]
```

---

## Applications

```cpp
// Matrix-matrix product and norm
#include "Matrix.h"
#include <iostream>

int main() {
    Matrix A(2, 2);
    A(0,0)=1; A(0,1)=2;
    A(1,0)=3; A(1,1)=4;

    Matrix B = A * A;
    std::cout << B;
    std::cout << "Frobenius norm: " << B.norm_frobenius() << "\n";
    std::cout << "Infinity norm:  " << B.norm_inf()       << "\n";
}
```

---

---

# MatrixGen

Utilities for generating test matrices.  
Header: `include/MatrixGen.h` — Namespace: `MatrixGen`

---

## Basic Examples

```cpp
#include "MatrixGen.h"
#include <iostream>

int main() {
    Matrix A = MatrixGen::make_random(3, 3);
    Matrix S = MatrixGen::make_symmetric(4);
    Matrix P = MatrixGen::make_spd(5);

    std::cout << A << "\n";
    std::cout << S << "\n";
    std::cout << P << "\n";
}
```

---

## Options

### `make_random`

```cpp
Matrix A = MatrixGen::make_random(
    rows,               // number of rows
    cols,               // number of cols
    integer_entries,    // false = real (default), true = integer
    lo,                 // lower bound (default -10.0)
    hi,                 // upper bound (default  10.0)
    seed                // 0 = non-deterministic (default), fixed = reproducible
);
```

```cpp
Matrix A = MatrixGen::make_random(3, 3);
Matrix B = MatrixGen::make_random(3, 3, true, -5.0, 5.0);
Matrix C = MatrixGen::make_random(3, 3, false, -1.0, 1.0, 42);
```

### `make_symmetric`

```cpp
Matrix S = MatrixGen::make_symmetric(
    n,                  // n x n
    integer_entries,    // default false
    lo,                 // default -10.0
    hi,                 // default  10.0
    seed                // default 0
);
```

```cpp
Matrix S  = MatrixGen::make_symmetric(4);
Matrix S2 = MatrixGen::make_symmetric(4, false, -1.0, 1.0, 7);
```

### `make_spd`

```cpp
Matrix P = MatrixGen::make_spd(
    n,                  // n x n
    integer_entries,    // default false
    lo,                 // default -5.0
    hi,                 // default  5.0
    seed                // default 0
);
```

```cpp
Matrix P  = MatrixGen::make_spd(5);
Matrix P2 = MatrixGen::make_spd(5, false, -3.0, 3.0, 99);
```

---

## Applications

```cpp
// Generate SPD system and solve it
#include "MatrixGen.h"
#include "LU.h"
#include <iostream>

int main() {
    Matrix A = MatrixGen::make_spd(4, false, -5.0, 5.0, 42);
    Vector b{1.0, 2.0, 3.0, 4.0};
    Vector x = solve(A, b);
    std::cout << "x = " << x << "\n";
}
```

---

---

# LU Factorization

Direct solver for `Ax = b` via PA = LU decomposition with partial pivoting.  
Header: `include/LU.h`

---

## Basic Examples

```cpp
#include "LU.h"
#include <iostream>

int main() {
    Matrix A(3, 3);
    A(0,0)=2; A(0,1)=1; A(0,2)=1;
    A(1,0)=4; A(1,1)=3; A(1,2)=3;
    A(2,0)=8; A(2,1)=7; A(2,2)=9;

    Vector b{4.0, 10.0, 26.0};

    Vector x = solve(A, b);
    std::cout << "x = " << x << "\n";
}
```

---

## Options

### `solve` — one-call convenience

```cpp
Vector x = solve(A, b);
// Copies A internally — caller's matrix is not modified.
```

### `lu_factorize` — explicit factorization

```cpp
Matrix LU = A;                      // preserve original
std::vector<size_t> P;
lu_factorize(LU, P);
// LU now holds L (strict lower triangle) and U (upper triangle + diagonal).
// P[i] = original row index now at position i.
```

### `lu_solve` — solve from existing factorization

```cpp
// Efficient when solving multiple systems with the same A
Matrix LU = A;
std::vector<size_t> P;
lu_factorize(LU, P);

Vector x1 = lu_solve(LU, P, b1);   // O(n²)
Vector x2 = lu_solve(LU, P, b2);   // O(n²) — factorization reused
```

---

## Applications

```cpp
// Solve and verify residual
#include "LU.h"
#include <iostream>

int main() {
    Matrix A(3, 3);
    A(0,0)=2; A(0,1)=1; A(0,2)=1;
    A(1,0)=4; A(1,1)=3; A(1,2)=3;
    A(2,0)=8; A(2,1)=7; A(2,2)=9;

    Vector b{4.0, 10.0, 26.0};
    Vector x = solve(A, b);

    Vector r = b - A * x;
    std::cout << "||r||_inf = " << r.linf_norm() << "\n";
    // Expected: < 1e-10
}
```

```cpp
// Compute three columns of A^{-1} by solving with identity columns
#include "LU.h"
#include "MatrixGen.h"

int main() {
    Matrix A = MatrixGen::make_spd(3, false, -5.0, 5.0, 42);

    Matrix LU = A;
    std::vector<size_t> P;
    lu_factorize(LU, P);

    Vector e1{1.0, 0.0, 0.0};
    Vector e2{0.0, 1.0, 0.0};
    Vector e3{0.0, 0.0, 1.0};

    Vector col1 = lu_solve(LU, P, e1);
    Vector col2 = lu_solve(LU, P, e2);
    Vector col3 = lu_solve(LU, P, e3);
    // col1, col2, col3 are the columns of A^{-1}
}
```

---

---

# SparseMatrix

CSR (Compressed Sparse Row) sparse matrix constructed from COO triplets.  
Header: `include/SparseMatrix.h`

---

## Basic Examples

```cpp
#include "SparseMatrix.h"
#include <iostream>
#include <vector>

int main() {
    //  A = [ 4  0  2 ]
    //      [ 0  3  0 ]
    //      [ 1  0  5 ]
    std::vector<std::size_t> row_idx = {0, 0, 1, 2, 2};
    std::vector<std::size_t> col_idx = {0, 2, 1, 0, 2};
    std::vector<double>      vals    = {4.0, 2.0, 3.0, 1.0, 5.0};

    SparseMatrix A(3, 3, row_idx, col_idx, vals);

    Vector x{1.0, 2.0, 3.0};
    Vector y = A.matvec(x);   // y = [10, 6, 16]

    std::cout << "nnz = " << A.nnz() << "\n";  // 5
    std::cout << y << "\n";                     // [10, 6, 16]
}
```

---

## Options

### Construction

```cpp
SparseMatrix A(
    rows,       // number of rows
    cols,       // number of columns
    row_idx,    // std::vector<std::size_t> — 0-based row index of each entry
    col_idx,    // std::vector<std::size_t> — 0-based column index of each entry
    vals        // std::vector<double>      — value of each entry
);
// All three vectors must have the same length (= nnz).
// No duplicate (row, col) pairs in the input.
```

The constructor converts COO to CSR in two passes: a prefix-sum over
row counts builds `row_ptr`, then a scatter loop places values and column
indices into their final positions. Construction is O(nnz + rows).

```cpp
// Diagonal matrix diag(1, 2, 3)
std::vector<std::size_t> ri = {0, 1, 2};
std::vector<std::size_t> ci = {0, 1, 2};
std::vector<double>      v  = {1.0, 2.0, 3.0};
SparseMatrix D(3, 3, ri, ci, v);
```

### Queries

```cpp
SparseMatrix A(3, 3, row_idx, col_idx, vals);

A.rows();   // number of rows
A.cols();   // number of columns
A.nnz();    // number of stored non-zeros
```

All three queries are O(1).

```cpp
std::cout << A.rows() << "\n";  // 3
std::cout << A.cols() << "\n";  // 3
std::cout << A.nnz()  << "\n";  // number of non-zero entries
```

### `matvec` — sparse matrix-vector product

```cpp
Vector y = A.matvec(x);   // y = A * x
// x must have size == A.cols()
// Returns a new Vector of size A.rows()
```

The SpMV kernel iterates over the CSR row ranges, touching exactly `nnz`
multiply-adds:

```cpp
//  A = [ 2  0  0 ]       x = [1, 2, 3]
//      [ 0  5  0 ]
//      [ 0  0  9 ]
std::vector<std::size_t> ri = {0, 1, 2};
std::vector<std::size_t> ci = {0, 1, 2};
std::vector<double>      v  = {2.0, 5.0, 9.0};
SparseMatrix D(3, 3, ri, ci, v);

Vector x{1.0, 2.0, 3.0};
Vector y = D.matvec(x);   // y = [2, 10, 27]
```

---

## Applications

```cpp
// 1D finite-difference stencil: tridiagonal system
// [-1  2 -1] on a 5-point grid
#include "SparseMatrix.h"
#include <vector>
#include <iostream>

int main() {
    const std::size_t n = 5;
    std::vector<std::size_t> ri, ci;
    std::vector<double>      v;

    for (std::size_t i = 0; i < n; ++i) {
        ri.push_back(i); ci.push_back(i); v.push_back(2.0);   // diagonal
        if (i > 0)     { ri.push_back(i); ci.push_back(i-1); v.push_back(-1.0); }
        if (i < n - 1) { ri.push_back(i); ci.push_back(i+1); v.push_back(-1.0); }
    }

    SparseMatrix T(n, n, ri, ci, v);
    std::cout << "nnz = " << T.nnz() << "\n";  // 13 for n=5

    Vector x{1.0, 1.0, 1.0, 1.0, 1.0};
    Vector y = T.matvec(x);
    std::cout << y << "\n";   // [1, 0, 0, 0, 1]  (interior rows cancel)
}
```

```cpp
// Verify sparse SpMV matches dense Matrix*Vector
#include "SparseMatrix.h"
#include "Matrix.h"
#include <cmath>
#include <cassert>

int main() {
    std::vector<std::size_t> ri = {0, 0, 1, 2, 2};
    std::vector<std::size_t> ci = {0, 2, 1, 0, 2};
    std::vector<double>      v  = {4.0, 2.0, 3.0, 1.0, 5.0};
    SparseMatrix S(3, 3, ri, ci, v);

    Matrix A(3, 3);
    A(0,0)=4.0; A(0,2)=2.0;
    A(1,1)=3.0;
    A(2,0)=1.0; A(2,2)=5.0;

    Vector x{1.0, 2.0, 3.0};
    Vector ys = S.matvec(x);
    Vector yd = A * x;

    for (std::size_t i = 0; i < 3; ++i)
        assert(std::fabs(ys[i] - yd[i]) < 1e-12);
    // passes: both give [10, 6, 16]
}
```

---
