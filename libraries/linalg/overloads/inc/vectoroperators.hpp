#ifndef LINALG_VECTOR_OPERATORS
#define LINALG_VECTOR_OPERATORS

#include "../../types/inc/matrix.hpp"
#include "../../types/inc/types.hpp"
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

template <size_t N>
DoubleArray<N> operator+(const DoubleArray<N>& vector, double scalar);
template <size_t N>
DoubleArray<N> operator+(double scalar, const DoubleArray<N>& vector);
template <size_t N>
DoubleArray<N> operator-(const DoubleArray<N>& vector, double scalar);

template <size_t N>
DoubleArray<N> operator+(const DoubleArray<N>& lhs, const DoubleArray<N>& rhs);
template <size_t N>
DoubleArray<N> operator-(const DoubleArray<N>& lhs, const DoubleArray<N>& rhs);

template <size_t N>
DoubleArray<N> operator*(const DoubleArray<N>& vector, double scalar);
template <size_t N>
DoubleArray<N> operator*(double scalar, const DoubleArray<N>& vector);
template <size_t N>
DoubleArray<N> operator/(const DoubleArray<N>& vector, double scalar);

template <size_t N>
double operator*(const DoubleArray<N>& lhs, const DoubleArray<N>& rhs); // Dot product
    
}

#include "vectoroperators_impl.hpp"

#endif