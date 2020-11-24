#ifndef CIE_CSG_CELL_HPP
#define CIE_CSG_CELL_HPP

// --- Internal Includes ---
#include "CSG/packages/primitives/inc/Primitive.hpp"
#include "CSG/packages/trees/inc/indexconverter.hpp"

// --- Utility Includes ---
#include "cieutils/packages/trees/inc/abstree.hpp"
#include "cieutils/packages/concepts/inc/container_concepts.hpp"
#include "cieutils/packages/types/inc/types.hpp"

// --- STL Includes ---
#include <memory>
#include <vector>
#include <memory>

namespace cie::csg {

// ---------------------------------------------------------
// CELL INTERFACE
// ---------------------------------------------------------

/**
 * Node interface of a tree of primitives, splittable around a single point.
 * The 'split' interface supplies a set of tuples necessary for constructing
 * a set of primitives (results of the split).
*/
template <concepts::PrimitiveType PrimitiveType>
class AbsCell : public PrimitiveType
{
public:
    using primitive_type                        = PrimitiveType;
    using cell_base_type                        = AbsCell<PrimitiveType>;
    
    using primitive_constructor_container       = std::vector<typename primitive_type::primitive_constructor_arguments>;
    using primitive_constructor_container_ptr   = std::shared_ptr<primitive_constructor_container>;

public:

    /**
     * Construct a cell by forwarding incoming arguments to the
     * primitive constructor.
    */
    template <class ...Args>
    AbsCell( Args&&... args );

    /**
     * Split overload that doesn't attempt to convert the coordinate container
     * to point_type (as it is already point_type).
    */
    primitive_constructor_container_ptr split( const typename AbsCell<PrimitiveType>::point_type& point );

protected:

    /**
     * Split the primitive around a point and return a container of tuples
     * that hold the arguments for constructing the subcells.
    */
    virtual primitive_constructor_container_ptr split_internal( const typename AbsCell<PrimitiveType>::point_type& point ) = 0;
};


// ---------------------------------------------------------
// BASIC CELL TYPES
// ---------------------------------------------------------

/* --- CubeCell --- */

template <concepts::PrimitiveType CubeType>
class CubeCell : public AbsCell<CubeType>
{
public:
    CubeCell(   const typename CubeCell<CubeType>::point_type& base, 
                typename CubeCell<CubeType>::coordinate_type length );

    typename CubeCell<CubeType>::primitive_constructor_container_ptr split( const typename CubeCell<CubeType>::point_type& r_point );

    /**
     * A cube will only produce subcubes when split if it's split at the
     * midpoint, so the actual split point is unnecessary and ignored.
    */
    typename CubeCell<CubeType>::primitive_constructor_container_ptr split( );

protected:
    virtual typename CubeCell<CubeType>::primitive_constructor_container_ptr split_internal( const typename CubeCell<CubeType>::point_type& point ) override;

protected:
    static const GridIndexConverter<CubeType::dimension> _childIndexConverter;
};



/* --- BoxCell --- */

template <concepts::PrimitiveType BoxType>
class BoxCell : public AbsCell<BoxType>
{
public:
    BoxCell(    const typename BoxCell<BoxType>::point_type& base, 
                const typename BoxCell<BoxType>::point_type& lengths );

protected:
    virtual typename BoxCell<BoxType>::primitive_constructor_container_ptr split_internal( const typename BoxCell<BoxType>::point_type& point ) override;

protected:
    static const GridIndexConverter<BoxType::dimension> _childIndexConverter;
};


} // namespace cie::csg

#include "CSG/packages/trees/impl/cell_impl.hpp"

#endif