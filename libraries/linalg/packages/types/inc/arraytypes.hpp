#ifndef LINALG_ARRAY_TYPES_HPP
#define LINALG_ARRAY_TYPES_HPP

// --- STL Includes ---
#include <array>
#include <cstddef>
#include <cstdint>

namespace cie {

typedef std::array<double, 2>   Point2;
typedef std::array<double, 3>   Point3;

template <size_t N>
using DoubleArray = std::array<double, N>;

template <size_t N>
using FloatArray = std::array<float, N>;

template <size_t N>
using IntArray = std::array<int, N>;

template <size_t N>
using UIntArray     = std::array<size_t, N>;
template <size_t N>
using UInt8Array    = std::array<uint8_t, N>;

template <size_t N>
using UInt16Array   = std::array<uint16_t, N>;

}

#endif