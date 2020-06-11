#ifndef CIE_CSG_PRIMITIVES_HPP
#define CIE_CSG_PRIMITIVES_HPP

// --- Internal Includes ---
#include "csgobject.hpp"

namespace cie {
namespace csg {


// ---------------------------------------------------------
// ABSTRACT PRIMITIVES
// ---------------------------------------------------------
template <  Size N, 
            concepts::NumericType CoordinateType = Double>
class Box : public Primitive<N,CoordinateType>
{
public:
    template <class ContainerType1, class ContainerType2>
    Box(    const ContainerType1& center, 
            const ContainerType2& lengths )
    requires concepts::ClassContainer<ContainerType1,CoordinateType>
                && concepts::ClassContainer<ContainerType2,CoordinateType>;

    const typename Box::point_type& center() const;
    const typename Box::point_type& lengths() const;
    typename Box::point_type& center();
    typename Box::point_type& lengths();

protected:
    typename Box::point_type     _center;
    typename Box::point_type     _lengths;
};


template <  Size N, 
            concepts::NumericType CoordinateType = Double>
class Cube : public Primitive<N,CoordinateType>
{
public:
    template <class ContainerType>
    Cube(   const ContainerType& base, 
            CoordinateType length )
    requires concepts::ClassContainer<ContainerType,CoordinateType>;

    const typename Cube::point_type& base() const;
    const typename Cube::coordinate_type& length() const;
    typename Cube::point_type& base();
    typename Cube::coordinate_type& length();

protected:
    typename Cube::point_type       _base;
    typename Cube::coordinate_type  _length;
};


// ---------------------------------------------------------
// SPECIALIZED BOOLEAN PRIMITIVES
// ---------------------------------------------------------
namespace boolean {


template <Size N, concepts::NumericType CoordinateType = Double>
class CSGCube : public Cube<N,CoordinateType>, public CSGObject<N,Bool,CoordinateType>
{
public:
    template <class ContainerType>
    CSGCube(    const ContainerType& base, 
                CoordinateType length )
        requires concepts::ClassContainer<ContainerType,CoordinateType>;

protected:
    virtual Bool at( const typename CSGCube::point_type& point ) const override;
};


template <Size N, concepts::NumericType CoordinateType = Double>
class CSGBox : public Box<N,CoordinateType>, public CSGObject<N,Bool,CoordinateType>
{
public:
    template <class ContainerType1, class ContainerType2>
    CSGBox(    const ContainerType1& center, 
            const ContainerType2& lengths )
        requires concepts::ClassContainer<ContainerType1,CoordinateType>
                    && concepts::ClassContainer<ContainerType2,CoordinateType>;

protected:
    virtual Bool at( const typename CSGBox::point_type& point ) const override;
};

}


// ---------------------------------------------------------
// SPECIALIZED NUMERIC PRIMITIVES
// ---------------------------------------------------------
namespace numeric {
// TODO
}


}
}

#include "primitives_impl.hpp"

#endif