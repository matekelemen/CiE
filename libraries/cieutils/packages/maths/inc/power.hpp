#ifndef CIE_UTILS_MATHS_POWER_HPP
#define CIE_UTILS_MATHS_POWER_HPP

// --- Internal Includes ---
#include "cieutils/packages/concepts/inc/basic_concepts.hpp"


namespace cie {


template <concepts::Integer BaseType, concepts::Integer ExponentType>
constexpr BaseType intPow( BaseType base, ExponentType exponent );


} // namespace cie

#include "cieutils/packages/maths/impl/power_impl.hpp"

#endif