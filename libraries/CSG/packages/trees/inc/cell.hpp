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
// BOOLEAN PRIMITIVE CELL TEMPLATES
// ---------------------------------------------------------
namespace boolean {
    

template <  Size dimension,
            class SelfType,
            concepts::NumericType CoordinateType = Double>
class CubeCellTemplate :    public AbsCell<boolean::CSGCube<dimension,CoordinateType>,CubeCellTemplate<dimension,SelfType,CoordinateType>>
{
public:
    template <class ContainerType>
    CubeCellTemplate(   const ContainerType& base, 
                        CoordinateType length )
    requires concepts::ClassContainer<ContainerType,CoordinateType>;

    typename CubeCellTemplate::child_container_type& split( );

protected:
    virtual typename CubeCellTemplate::child_container_type& split_internal( const typename CubeCellTemplate::point_type& point ) override;
};


template <  Size dimension,
            class SelfType,
            concepts::NumericType CoordinateType = Double>
class BoxCellTemplate :     public AbsCell<boolean::CSGBox<dimension,CoordinateType>,BoxCellTemplate<dimension,SelfType,CoordinateType>>
{
public:
    template <class ContainerType1, class ContainerType2>
    BoxCellTemplate(const ContainerType1& base, 
                    const ContainerType2& lengths )
    requires concepts::ClassContainer<ContainerType1,CoordinateType>
                && concepts::ClassContainer<ContainerType2,CoordinateType>;

protected:
    virtual typename BoxCellTemplate::child_container_type& split_internal( const typename BoxCellTemplate::point_type& point ) override;
};


// ---------------------------------------------------------
// BOOLEAN PRIMITIVE CELL TEMPLATES
// ---------------------------------------------------------

template <  Size Dimension,
            concepts::NumericType CoordinateType = Double >
class CubeCell : public CubeCellTemplate<Dimension,CubeCell<Dimension,CoordinateType>,CoordinateType>
{
public:
    template <class ContainerType>
    CubeCell(   const ContainerType& base, 
                CoordinateType length )
    requires concepts::ClassContainer<ContainerType,CoordinateType> :
        CubeCellTemplate<Dimension,CubeCell<Dimension,CoordinateType>,CoordinateType>(base,length) 
    {}
};


template <  Size Dimension,
            concepts::NumericType CoordinateType = Double >
class BoxCell : public BoxCellTemplate<Dimension,BoxCell<Dimension,CoordinateType>,CoordinateType>
{
public:
    template <class ContainerType1, class ContainerType2>
    BoxCell(    const ContainerType1& base, 
                const ContainerType2& lengths )
    requires concepts::ClassContainer<ContainerType1,CoordinateType>
                && concepts::ClassContainer<ContainerType2,CoordinateType> :
        BoxCellTemplate<Dimension,BoxCell<Dimension,CoordinateType>,CoordinateType>(base,lengths)
    {}
};


} // namespace boolean


}

#include "../impl/cell_impl.hpp"

#endif