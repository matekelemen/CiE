#ifndef CIE_CSG_CELL_HPP
#define CIE_CSG_CELL_HPP

// --- Internal Includes ---
#include "../../primitives/inc/primitives.hpp"

// --- Utility Includes ---
#include <cieutils/trees.hpp>
#include <cieutils/concepts.hpp>
#include <cieutils/types.hpp>

// --- STL Includes ---
#include <memory>

namespace cie::csg {

// ---------------------------------------------------------
// ABSTRACT CELL
// ---------------------------------------------------------

template <  class PrimitiveType,
            class SelfType  >
class AbsCell : public PrimitiveType,
                public utils::AbsTree<std::vector,SelfType>
{
public:
    typedef PrimitiveType                   primitive_type;
    typedef AbsCell<PrimitiveType,SelfType> cell_base_type;

public:
    template <class ...Args>
    AbsCell( Args&&... args );

    template <concepts::NumericContainer PointType>
    typename AbsCell::child_container_type& split( const PointType& point );
    typename AbsCell::child_container_type& split( const typename AbsCell::point_type& point );

protected:
    virtual typename AbsCell::child_container_type& split_internal( const typename AbsCell::point_type& point ) = 0;
};


// ---------------------------------------------------------
// BOOLEAN PRIMITIVE CELLS
// ---------------------------------------------------------
namespace boolean {
    

template <  Size dimension,
            concepts::NumericType CoordinateType = Double>
class CubeCell :    public AbsCell<boolean::CSGCube<dimension,CoordinateType>,CubeCell<dimension,CoordinateType>>
{
public:
    template <class ContainerType>
    CubeCell(   const ContainerType& base, 
                CoordinateType length )
    requires concepts::ClassContainer<ContainerType,CoordinateType>;

    typename CubeCell::child_container_type& split( );

protected:
    virtual typename CubeCell::child_container_type& split_internal( const typename CubeCell::point_type& point ) override;
};


template <  Size dimension, 
            concepts::NumericType CoordinateType = Double>
class BoxCell :     public AbsCell<boolean::CSGBox<dimension,CoordinateType>,BoxCell<dimension,CoordinateType>>
{
public:
    template <class ContainerType1, class ContainerType2>
    BoxCell(const ContainerType1& base, 
            const ContainerType2& lengths )
    requires concepts::ClassContainer<ContainerType1,CoordinateType>
                && concepts::ClassContainer<ContainerType2,CoordinateType>;

protected:
    virtual typename BoxCell::child_container_type& split_internal( const typename BoxCell::point_type& point ) override;
};


}


}

#include "../impl/cell_impl.hpp"

#endif