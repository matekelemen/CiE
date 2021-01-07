#ifndef CIE_CSG_TREES_CELL_HPP
#define CIE_CSG_TREES_CELL_HPP

// --- Internal Includes ---
#include "CSG/packages/trees/inc/AbsCell.hpp"
#include "CSG/packages/primitives/inc/Cube.hpp"
#include "CSG/packages/primitives/inc/Box.hpp"


namespace cie::csg {


/* --- Cube cell --- */

template <concepts::Cube PrimitiveType>
class CubeCell : public AbsCell<PrimitiveType>
{
public:
    CubeCell(   const typename CubeCell<PrimitiveType>::point_type& base, 
                typename CubeCell<PrimitiveType>::coordinate_type length );

    typename CubeCell<PrimitiveType>::primitive_constructor_container_ptr split( const typename CubeCell<PrimitiveType>::point_type& r_point );

    /**
     * A cube will only produce subcubes when split if it's split at the
     * midpoint, so the actual split point is unnecessary and ignored.
    */
    typename CubeCell<PrimitiveType>::primitive_constructor_container_ptr split( );

protected:
    virtual typename CubeCell<PrimitiveType>::primitive_constructor_container_ptr split_internal( const typename CubeCell<PrimitiveType>::point_type& point ) override;

protected:
    static const GridIndexConverter<PrimitiveType::dimension> _childIndexConverter;
};



/* --- BoxCell --- */

template <concepts::Box PrimitiveType>
class BoxCell : public AbsCell<PrimitiveType>
{
public:
    BoxCell( const typename BoxCell<PrimitiveType>::point_type& base, 
             const typename BoxCell<PrimitiveType>::point_type& lengths );

protected:
    virtual typename BoxCell<PrimitiveType>::primitive_constructor_container_ptr split_internal( const typename BoxCell<PrimitiveType>::point_type& point ) override;

protected:
    static const GridIndexConverter<PrimitiveType::dimension> _childIndexConverter;
};


} // namespace cie::csg

#include "CSG/packages/trees/impl/Cell_impl.hpp"

#endif