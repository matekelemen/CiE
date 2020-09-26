#ifndef CIE_UTILS_MATHS_POWER_IMPL_HPP
#define CIE_UTILS_MATHS_POWER_IMPL_HPP

// --- Internal Includes ---
#include "cieutils/packages/macros/inc/assertions.hpp"


namespace cie {


template <concepts::Integer NT>
constexpr NT intPow( NT base, NT exponent )
{
    CIE_ASSERT(
        exponent >= 0,
        "Negative exponents for integer bases yield non-integer results!"
    )

    if (exponent == 0)
        return 1;
    else if (exponent == 1)
        return base;

    NT output = intPow(base, exponent/2);

    if (exponent%2 == 0)
        return output * output;
    else
        return output * output * base;
}


} // namespace cie


#endif