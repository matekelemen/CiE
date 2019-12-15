#ifndef LINALG_VECTOR_TYPES_HPP
#define LINALG_VECTOR_TYPES_HPP

#include <vector>
#include <stdint.h>

namespace cie {

typedef std::vector<uint8_t>    UInt8Vector;
typedef std::vector<size_t>     UIntVector;
typedef std::vector<int>        IntVector;
typedef std::vector<double>     DoubleVector;

}

#endif