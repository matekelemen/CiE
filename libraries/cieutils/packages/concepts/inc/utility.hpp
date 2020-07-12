#ifndef CIE_CIEUTILS_CONCEPTS_UTILITY_HPP
#define CIE_CIEUTILS_CONCEPTS_UTILITY_HPP

// --- Internal Includes ---
#include "./container_concepts.hpp"
#include "../../types/inc/types.hpp"

// --- STL Includes ---
#include <cassert>


namespace cie::utils {


template <concepts::ResizableContainer ContainerType>
inline void setContainerSize( ContainerType& container, Size size )
{ container.resize(size); }


template <concepts::FixedSizeContainer ContainerType>
inline void setContainerSize( ContainerType& container, Size size )
{ assert(container.size() == size); }


}

#endif