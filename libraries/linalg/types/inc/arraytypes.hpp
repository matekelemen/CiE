#ifndef LINALG_ARRAY_TYPES_HPP
#define LINALG_ARRAY_TYPES_HPP

#include <array>

typedef std::array<double, 2>   Point2;
typedef std::array<double, 3>   Point3;

template <size_t N>
using DoubleArray = std::array<double, N>;

template <size_t N>
using IntArray = std::array<int, N>;

template <size_t N>
using UIntArray = std::array<uint, N>;

#endif