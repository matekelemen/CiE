#ifndef CSG_NTREE_UTILITIES_IMPL_HPP
#define CSG_NTREE_UTILITIES_IMPL_HPP

#include <string>

namespace cie {
namespace csg {


template <uint8_t M>
void baseN(size_t base_10, uint8_t base, UInt8Array<M>& base_N)
{
    for (int i = 0; i < M; ++i)
    {
        if (base_10 == 0)
        {
            base_N[i]   = 0;
        }
        else
        {
            base_N[i]   = base_10 % base;
            base_10     /= base;
        }
    }
    if (base_10 != 0)
    {
        throw std::overflow_error("Error converting to base " + std::to_string(base));
    }
}


template <uint8_t M>
UInt8Array<M> baseN(size_t base_10, uint8_t base)
{
    UInt8Array<M> result;
    baseN<M>(base_10, base, result);
    return result;
}


template <uint8_t M>
size_t base10(const UInt8Array<M>& base_N, uint8_t base)
{
    size_t power = 1;
    size_t base_10 = 0;
    for (uint8_t i = 0; i < M; ++i)
    {
        base_10 += power*base_N[i];
        power *= base;
    }
    return base_10;
}


}
}


#endif