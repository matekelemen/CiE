#ifndef CIE_UTILS_BIT_OPERATIONS_HPP
#define CIE_UTILS_BIT_OPERATIONS_HPP

// --- Internal Includes ---
#include "cieutils/packages/concepts/inc/basic_concepts.hpp"


namespace cie::utils {


template <concepts::Integer IntegerType, concepts::Integer IndexType = Size>
IntegerType flipBit( IntegerType integer, IndexType bitIndex );



} // namespace cie::utils

#include "cieutils/packages/maths/impl/bit_impl.hpp"

#endif