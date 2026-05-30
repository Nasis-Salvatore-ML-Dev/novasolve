# NovaSolve

A from-scratch C++ numerical library for solving PDEs — classically via
finite difference and iterative linear algebra, and via physics-informed
neural network inference — with direct performance comparisons between the two.

C++ numerical computing library built for scientific computing and HPC environments. Active development — Iterations 1–3 complete, remaining iterations in progress alongside production ML work.

## Build

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/test_lu        # LU factorization tests
./build/test_matrix    # dense matrix tests
./build/test_vector    # dense vector tests
./build/test_sparse    # sparse matrix tests
```

Requires: C++20, CMake ≥ 3.15.

## LU Factorization

`LU.h` implements dense direct solving via PA = LU decomposition with partial
pivoting. At each elimination step the row with the largest absolute value in
the current column is swapped to the pivot position — preventing small pivots
from amplifying rounding errors and guaranteeing numerical stability for any
non-singular system. L and U are stored compactly in-place (the unit diagonal
of L is implicit), and the permutation is recorded as a std::vector<size_t>.
Solving Ax = b reduces to forward substitution through L followed by back
substitution through U, both O(n²) after the O(n³) factorization.

## Sparse Matrix (CSR)

`SparseMatrix.h` implements CSR (Compressed Sparse Row) storage built
from COO triplets (row indices, column indices, values). Only non-zero
entries are stored, reducing memory from O(n²) to O(nnz). The SpMV
kernel y = A*x loops only over stored non-zeros per row, making it
the foundational primitive for all iterative solvers.

## Iterations

| #   | Module                  | Status     |
| --- | ----------------------- | ---------- |
| 1   | Dense Vector & Matrix   | ✓ Complete |
| 2   | LU Factorization        | ✓ Complete |
| 3   | Sparse Storage (CSR)    | ✓ Complete |
| 4   | Iterative Solvers (CG)  | Planned    |
| 5   | PDE Discretization (FD) | Planned    |
| 6   | Time-Dependent PDE      | Planned    |
| 7   | PINN Inference Engine   | Planned    |
| 8   | Benchmarks & OpenMP     | Planned    |
