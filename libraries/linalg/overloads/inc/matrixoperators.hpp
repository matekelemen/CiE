#ifndef LINALG_MATRIX_OPERATORS
#define LINALG_MATRIX_OPERATORS

#include "../../types/inc/arraytypes.hpp"
#include "../../types/inc/vectortypes.hpp"
#include "../../types/inc/matrix.hpp"
#include <string>
#include <algorithm>

namespace linalg {

Matrix operator+(const Matrix& matrix, double scalar);
Matrix operator+(double scalar, const Matrix& matrix);
Matrix operator-(const Matrix& matrix, double scalar);

Matrix operator+(const Matrix& lhs, const Matrix& rhs);
Matrix operator-(const Matrix& lhs, const Matrix& rhs);

Matrix operator*(const Matrix& matrix, double scalar);
Matrix operator*(double scalar, const Matrix& matrix);
Matrix operator/(const Matrix& matrix, double scalar);

DoubleVector operator*(const DoubleVector& vector, const Matrix& matrix);
DoubleVector operator*(const Matrix& matrix, const DoubleVector& vector);

template <size_t N>
DoubleArray<N> operator*(const DoubleArray<N>& vector, const Matrix& matrix);
template <size_t N>
DoubleArray<N> operator*(const Matrix& matrix, const DoubleArray<N>& vector);

Matrix operator*(const Matrix& lhs, const Matrix& rhs);

} // namespace linalg

#include "matrixoperators_impl.hpp"

#endif