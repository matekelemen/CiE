#ifndef LINALG_MATRIX_OPERATORS
#define LINALG_MATRIX_OPERATORS

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

Vector operator*(const Vector& vector, const Matrix& matrix);
Vector operator*(const Matrix& matrix, const Vector& vector);

Matrix operator*(const Matrix& lhs, const Matrix& rhs);

}

#endif