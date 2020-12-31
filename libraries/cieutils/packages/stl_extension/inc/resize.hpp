#ifndef CIE_CIEUTILS_CONCEPTS_UTILITY_HPP
#define CIE_CIEUTILS_CONCEPTS_UTILITY_HPP

// --- Internal Includes ---
#include "cieutils/packages/macros/inc/checks.hpp"
#include "cieutils/packages/concepts/inc/container_concepts.hpp"
#include "cieutils/packages/types/inc/types.hpp"

// --- STL Includes ---
#include <cassert>


namespace cie::utils {


template <concepts::ResizableContainer ContainerType>
inline void resize( ContainerType& container, Size size )
{ container.resize(size); }


template <concepts::FixedSizeContainer ContainerType>
inline void resize( ContainerType& container, Size size )
{ CIE_OUT_OF_RANGE_CHECK( container.size() == size ) }


template <concepts::ReservableContainer ContainerType>
inline void reserve( ContainerType& container, Size size )
{ container.reserve(size); }


template <concepts::FixedSizeContainer ContainerType>
inline void reserve( ContainerType& container, Size size )
{ CIE_OUT_OF_RANGE_CHECK( container.size() >= size ) }


} // namespace cie::utils

#endif