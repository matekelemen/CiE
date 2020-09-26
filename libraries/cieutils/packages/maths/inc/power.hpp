#ifndef CIE_UTILS_MATHS_POWER_HPP
#define CIE_UTILS_MATHS_POWER_HPP

// --- Internal Includes ---
#include "cieutils/packages/concepts/inc/basic_concepts.hpp"


namespace cie {


template <concepts::Integer NT>
constexpr NT intPow( NT base, NT exponent );


} // namespace cie

#include "cieutils/packages/maths/impl/power_impl.hpp"

#endif