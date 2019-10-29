#ifndef LINALG_VECTOR_OPERATORS
#define LINALG_VECTOR_OPERATORS

#include "../../types/inc/matrix.hpp"
#include <algorithm>
#include <numeric>

namespace linalg {

Vector operator+(const Vector& vector, double scalar);
Vector operator+(double scalar, const Vector& vector);
Vector operator-(const Vector& vector, double scalar);

Vector operator+(const Vector& lhs, const Vector& rhs);
Vector operator-(const Vector& lhs, const Vector& rhs);

Vector operator*(const Vector& vector, double scalar);
Vector operator*(double scalar, const Vector& vector);
Vector operator/(const Vector& vector, double scalar);

double operator*(const Vector& lhs, const Vector& rhs); // Dot product
    
}

#endif