#ifndef CIE_CSG_TREES_ABS_CELL_IMPL_HPP
#define CIE_CSG_TREES_ABS_CELL_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/exceptions/inc/exception.hpp"
#include "cieutils/packages/stl_extension/inc/resize.hpp"
#include "cieutils/packages/maths/inc/power.hpp"
#include "cieutils/packages/macros/inc/checks.hpp"

// --- Internal Includes ---
#include "CSG/packages/trees/inc/indexconverter.hpp"

// --- STL Includes ---
#include <iostream>

namespace cie::csg {

template <concepts::Primitive PrimitiveType>
template <class ...Args>
AbsCell<PrimitiveType>::AbsCell( Args&&... args ) :
    PrimitiveType( std::forward<Args>(args) ... )
{
}



template <concepts::Primitive PrimitiveType>
inline typename AbsCell<PrimitiveType>::primitive_constructor_container_ptr
AbsCell<PrimitiveType>::split( const typename AbsCell<PrimitiveType>::point_type& point )
{
    return this->split_internal(point);
}


} // namespace cie::csg

#endif