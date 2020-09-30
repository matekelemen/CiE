#ifndef CIE_CSG_CELL_HPP
#define CIE_CSG_CELL_HPP

// --- Internal Includes ---
#include "CSG/packages/primitives/inc/primitives.hpp"
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
// ABSTRACT CELL
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
     * Convert a container of coordinates to point_type,
     * and call the internal split function.
    */
    template <concepts::NumericContainer PointType>
    primitive_constructor_container_ptr split( const PointType& point );

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
// BOOLEAN PRIMITIVE CELLS
// ---------------------------------------------------------
namespace boolean {
    

template <  Size dimension,
            concepts::NumericType CoordinateType = Double>
class CubeCell :
    public AbsCell<boolean::CSGCube<dimension,CoordinateType>>
{
public:
    template <class ContainerType>
    CubeCell(   const ContainerType& base, 
                CoordinateType length )
    requires concepts::ClassContainer<ContainerType,CoordinateType>;

    /**
     * A cube will only produce subcubes when split if it's split at the
     * midpoint, so this overload doesn't allow a point to be passed and
     * shadows other AbsCell::split members.
    */
    typename CubeCell<dimension,CoordinateType>::primitive_constructor_container_ptr split( );

protected:
    virtual typename CubeCell<dimension,CoordinateType>::primitive_constructor_container_ptr split_internal( const typename CubeCell<dimension, CoordinateType>::point_type& point ) override;

protected:
    static const GridIndexConverter<dimension> _childIndexConverter;
};



template <  Size dimension,
            concepts::NumericType CoordinateType = Double>
class BoxCell :
    public AbsCell<boolean::CSGBox<dimension,CoordinateType>>
{
public:
    template <class ContainerType1, class ContainerType2>
    BoxCell(    const ContainerType1& base, 
                const ContainerType2& lengths )
    requires concepts::ClassContainer<ContainerType1,CoordinateType>
                && concepts::ClassContainer<ContainerType2,CoordinateType>;

protected:
    virtual typename BoxCell<dimension,CoordinateType>::primitive_constructor_container_ptr split_internal( const typename BoxCell<dimension, CoordinateType>::point_type& point ) override;

protected:
    static const GridIndexConverter<dimension> _childIndexConverter;
};

} // namespace boolean


} // namespace cie::csg

#include "CSG/packages/trees/impl/cell_impl.hpp"

#endif