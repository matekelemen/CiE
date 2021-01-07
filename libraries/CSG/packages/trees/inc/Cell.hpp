#ifndef CIE_CSG_TREES_CELL_HPP
#define CIE_CSG_TREES_CELL_HPP

// --- Internal Includes ---
#include "CSG/packages/trees/inc/AbsCell.hpp"
#include "CSG/packages/trees/inc/indexconverter.hpp"
#include "CSG/packages/primitives/inc/Cube.hpp"
#include "CSG/packages/primitives/inc/Box.hpp"


namespace cie::csg {


template <concepts::Cube PrimitiveType>
class Cell<PrimitiveType> : public AbsCell<PrimitiveType>
{
public:
    Cell( const typename Cell<PrimitiveType>::point_type& base, 
          typename Cell<PrimitiveType>::coordinate_type length );

    typename Cell<PrimitiveType>::primitive_constructor_container_ptr split( const typename Cell<PrimitiveType>::point_type& r_point );

    /**
     * A cube will only produce subcubes when split if it's split at the
     * midpoint, so the actual split point is unnecessary and ignored.
    */
    typename Cell<PrimitiveType>::primitive_constructor_container_ptr split( );

protected:
    virtual typename Cell<PrimitiveType>::primitive_constructor_container_ptr split_internal( const typename Cell<PrimitiveType>::point_type& point ) override;

protected:
    static const GridIndexConverter<PrimitiveType::dimension> _childIndexConverter;
};



/* --- Box cell --- */

template <concepts::Box PrimitiveType>
class Cell<PrimitiveType> : public AbsCell<PrimitiveType>
{
public:
    Cell( const typename Cell<PrimitiveType>::point_type& base, 
          const typename Cell<PrimitiveType>::point_type& lengths );

protected:
    virtual typename Cell<PrimitiveType>::primitive_constructor_container_ptr split_internal( const typename Cell<PrimitiveType>::point_type& point ) override;

protected:
    static const GridIndexConverter<PrimitiveType::dimension> _childIndexConverter;
};


} // namespace cie::csg

#include "CSG/packages/trees/impl/Cell_impl.hpp"

#endif