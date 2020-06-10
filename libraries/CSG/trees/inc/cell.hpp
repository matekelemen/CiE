#ifndef CIE_CSG_CELL_HPP
#define CIE_CSG_CELL_HPP

// --- Internal Includes ---
#include "../../primitives/inc/primitives.hpp"

// --- Utility Includes ---
#include <cieutils/types.hpp>
#include <cieutils/concepts.hpp>
#include <cieutils/trees.hpp>

// --- STD Includes ---
#include <memory>

namespace cie::csg {
// ---------------------------------------------------------
// ABSTRACT CELL
// ---------------------------------------------------------
template <  Size dimension,
            class ChildType,
            concepts::NumericType CoordinateType = Double>
class Cell : public Primitive<dimension,CoordinateType>
{
public:
    typedef ChildType                               child_type;
    typedef std::vector<std::shared_ptr<ChildType>> child_container_type;

    virtual Bool isInside( const typename Cell::point_type& point ) const;
    virtual child_container_type& split( const typename Cell::point_type& point ) = 0;

    const child_container_type& children() const;
    child_container_type& children();

protected:
    child_container_type _children;
};


// ---------------------------------------------------------
// PRIMITIVE CELLS
// ---------------------------------------------------------
template <Size dimension, concepts::NumericType CoordinateType = Double>
class BoxCell : public Cell<dimension,BoxCell<dimension,CoordinateType>,CoordinateType>, public Box<dimension,CoordinateType>
{
public:
    template <class ContainerType1, class ContainerType2>
    BoxCell(const ContainerType1& center, 
            const ContainerType2& lengths )
    requires concepts::ClassContainer<ContainerType1,CoordinateType>
                && concepts::ClassContainer<ContainerType2,CoordinateType>;

    virtual Bool isInside( const typename BoxCell::point_type& point ) const override;
    virtual typename BoxCell::child_container_type& split( const typename BoxCell::point_type& point ) override;
};





}

#include "cell_impl.hpp"

#endif