#ifndef CIE_UTILS_MATHS_BASE_HPP
#define CIE_UTILS_MATHS_BASE_HPP

// --- Internal Includes ---
#include "cieutils/packages/concepts/inc/basic_concepts.hpp"
#include "cieutils/packages/types/inc/types.hpp"

// --- STL Includes ---
#include <array>


namespace cie::utils {


template <  Size NDigits,
            concepts::Integer NT >
constexpr void
baseN(  NT base10,
        NT base,
        std::array<NT,NDigits>& output );


template <  Size NDigits,
            concepts::Integer NT >
constexpr std::array<NT,NDigits>
baseN(  NT base10,
        NT base );


template <  Size NDigits,
            concepts::Integer NT >
constexpr NT
base10( const std::array<NT,NDigits>& base_N,
        NT base );


template <  Size NDigits,
            concepts::Integer NT >
constexpr NT
reinterpretBase(    const std::array<NT,NDigits>& base_N,
                    NT newBase );



} // namespace cie::utils

#include "cieutils/packages/maths/impl/base_impl.hpp"

#endif