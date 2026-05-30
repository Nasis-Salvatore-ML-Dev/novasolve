#pragma once

#include <cassert>
#include <cstddef>
#include <vector>
#include "Vector.h"

// CSR sparse matrix constructed from COO triplets.
// No duplicate (row, col) entries in the input triplets.
class SparseMatrix {
public:
    SparseMatrix(std::size_t rows, std::size_t cols,
                 const std::vector<std::size_t>& row_idx,
                 const std::vector<std::size_t>& col_idx,
                 const std::vector<double>&      vals)
        : m_rows{rows}
        , m_cols{cols}
        , m_values(vals.size())
        , m_col_indices(vals.size())
        , m_row_ptr(rows + 1, 0)
    {
        const std::size_t n = vals.size();
        assert(row_idx.size() == n && col_idx.size() == n);

        for (std::size_t k = 0; k < n; ++k) {
            assert(row_idx[k] < m_rows && col_idx[k] < m_cols);
            m_row_ptr[row_idx[k] + 1]++;
        }

        for (std::size_t i = 1; i <= m_rows; ++i)
            m_row_ptr[i] += m_row_ptr[i - 1];

        std::vector<std::size_t> pos(m_row_ptr.begin(), m_row_ptr.end());
        for (std::size_t k = 0; k < n; ++k) {
            const std::size_t r   = row_idx[k];
            const std::size_t p   = pos[r]++;
            m_values[p]      = vals[k];
            m_col_indices[p] = col_idx[k];
        }
    }

    [[nodiscard]] std::size_t rows() const noexcept { return m_rows; }
    [[nodiscard]] std::size_t cols() const noexcept { return m_cols; }
    [[nodiscard]] std::size_t nnz()  const noexcept { return m_values.size(); }

    Vector matvec(const Vector& x) const {
        assert(x.size() == m_cols && "SparseMatrix::matvec: dimension mismatch");
        Vector y(m_rows);
        for (std::size_t i = 0; i < m_rows; ++i)
            for (std::size_t k = m_row_ptr[i]; k < m_row_ptr[i + 1]; ++k)
                y[i] += m_values[k] * x[m_col_indices[k]];
        return y;
    }

private:
    std::size_t              m_rows;
    std::size_t              m_cols;
    std::vector<double>      m_values;
    std::vector<std::size_t> m_col_indices;
    std::vector<std::size_t> m_row_ptr;
};
