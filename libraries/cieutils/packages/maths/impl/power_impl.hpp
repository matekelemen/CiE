#ifndef CIE_UTILS_MATHS_POWER_IMPL_HPP
#define CIE_UTILS_MATHS_POWER_IMPL_HPP

// --- Internal Includes ---
#include "cieutils/packages/macros/inc/assertions.hpp"


namespace cie {


template <concepts::Integer BaseType, concepts::Integer ExponentType>
constexpr BaseType intPow( BaseType base, ExponentType exponent )
{
    CIE_ASSERT(
        exponent >= 0,
        "Negative exponents for integer bases yield non-integer results!"
    )

    if (exponent == 0)
        return 1;
    else if (exponent == 1)
        return base;

    BaseType output = intPow(base, exponent/2);

    if (exponent%2 == 0)
        return output * output;
    else
        return output * output * base;
}


} // namespace cie


#endif