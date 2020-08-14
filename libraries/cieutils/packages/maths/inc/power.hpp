#ifndef CIE_UTILS_MATHS_POWER_HPP
#define CIE_UTILS_MATHS_POWER_HPP

// --- Internal Includes ---
#include "../../concepts/inc/basic_concepts.hpp"


namespace cie {


template <concepts::Integer NT>
constexpr NT intPow( NT base, NT exponent );


} // namespace cie

#include "../impl/power_impl.hpp"

#endif