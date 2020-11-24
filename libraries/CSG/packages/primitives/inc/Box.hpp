#ifndef CIE_CSG_PRIMITIVES_BOX_HPP
#define CIE_CSG_PRIMITIVES_BOX_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/container_concepts.hpp"

// --- Internal Includes ---
#include "CSG/packages/primitives/inc/Primitive.hpp"

// --- STL Includes ---
#include <tuple>


namespace cie::csg {


/**
 * Box template
*/
template < Size Dimension,
           concepts::NumericType CoordinateType = Double >
class Box : public AbsPrimitive<Dimension,CoordinateType>
{
public:
    using primitive_constructor_arguments
        = std::tuple<typename Box::point_type, typename Box::point_type>;

public:
    template <class ContainerType1, class ContainerType2>
    requires concepts::ClassContainer<ContainerType1,CoordinateType>
             && concepts::ClassContainer<ContainerType2,CoordinateType>
    Box( const ContainerType1& r_base, 
         const ContainerType2& r_lengths );

    virtual Bool isDegenerate() const override;

    const typename Box<Dimension,CoordinateType>::point_type& base() const;
    const typename Box<Dimension,CoordinateType>::point_type& lengths() const;
    typename Box<Dimension,CoordinateType>::point_type& base();
    typename Box<Dimension,CoordinateType>::point_type& lengths();

protected:
    typename Box<Dimension,CoordinateType>::point_type _base;
    typename Box<Dimension,CoordinateType>::point_type _lengths;
};


namespace boolean {

/**
 * Box with point membership test
*/
template <Size Dimension, concepts::NumericType CoordinateType = Double>
class Box :
    public cie::csg::Box<Dimension,CoordinateType>,
    public CSGObject<Dimension,Bool,CoordinateType>
{
public:
    template <class ContainerType1, class ContainerType2>
    requires concepts::ClassContainer<ContainerType1,CoordinateType>
             && concepts::ClassContainer<ContainerType2,CoordinateType>
    Box( const ContainerType1& r_base, 
         const ContainerType2& r_lengths );

protected:
    virtual Bool at( const typename Box<Dimension,CoordinateType>::point_type& r_point ) const override;
};

} // namespace boolean


} // namespace cie::csg

#include "CSG/packages/primitives/impl/Box_impl.hpp"

#endif