#ifndef LINALG_VECTOR_OPERATORS
#define LINALG_VECTOR_OPERATORS

#include "../../types/inc/matrix.hpp"
#include "../../types/inc/vectortypes.hpp"
#include "../../types/inc/arraytypes.hpp"
#include <algorithm>
#include <numeric>


DoubleVector operator+(const DoubleVector& vector, double scalar);
DoubleVector operator+(double scalar, const DoubleVector& vector);
DoubleVector operator-(const DoubleVector& vector, double scalar);

DoubleVector operator+(const DoubleVector& lhs, const DoubleVector& rhs);
DoubleVector operator-(const DoubleVector& lhs, const DoubleVector& rhs);

DoubleVector operator*(const DoubleVector& vector, double scalar);
DoubleVector operator*(double scalar, const DoubleVector& vector);
DoubleVector operator/(const DoubleVector& vector, double scalar);

double operator*(const DoubleVector& lhs, const DoubleVector& rhs); // Dot product

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


#include "vectoroperators_impl.hpp"

#endif