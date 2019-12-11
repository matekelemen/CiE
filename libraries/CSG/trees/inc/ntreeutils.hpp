#ifndef CSG_NTREEUTILS_HPP
#define CSG_NTREEUTILS_HPP

#include <stdint.h>
#include <functional>

#include "linalgtypes.hpp"

namespace cie {
namespace csg {


template <uint8_t N>
using GeometryFunction = std::function<uint8_t(const DoubleArray<N>&)>;


const size_t intPow(int base, uint8_t exponent);


template <uint8_t M>
void baseN(size_t base_10, uint8_t base, UInt8Array<M>& base_N);


template <uint8_t M>
UInt8Array<M> baseN(size_t base_10, uint8_t base);


template <uint8_t M>
size_t base10(const UInt8Array<M>& base_N, uint8_t base);


}
}

#include "ntreeutils_impl.hpp"

#endif