#ifndef CIE_CSG_PRIMITIVES_CUBE_HPP
#define CIE_CSG_PRIMITIVES_CUBE_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/container_concepts.hpp"

// --- Internal Includes ---
#include "CSG/packages/primitives/inc/Primitive.hpp"

// --- STL Includes ---
#include <tuple>


namespace cie::csg {


/**
 * Cube template
*/
template < Size Dimension,
           concepts::NumericType CoordinateType = Double>
class Cube : public AbsPrimitive<Dimension,CoordinateType>
{
public:
    using primitive_constructor_arguments
        = std::tuple<typename Cube::point_type, typename Cube::coordinate_type>;

public:
    template <class ContainerType>
    requires concepts::ClassContainer<ContainerType,CoordinateType>
    Cube( const ContainerType& base,
          CoordinateType length );

    virtual Bool isDegenerate() const override;

    const typename Cube<Dimension,CoordinateType>::point_type& base() const;
    const typename Cube<Dimension, CoordinateType>::coordinate_type& length() const;
    typename Cube<Dimension, CoordinateType>::point_type& base();
    typename Cube<Dimension, CoordinateType>::coordinate_type& length();

protected:
    typename Cube<Dimension, CoordinateType>::point_type       _base;
    typename Cube<Dimension, CoordinateType>::coordinate_type  _length;
};


namespace boolean {

/**
 * Cube with point membership test
*/
template <Size Dimension, concepts::NumericType CoordinateType = Double>
class Cube :
    public cie::csg::Cube<Dimension,CoordinateType>,
    public CSGObject<Dimension,Bool,CoordinateType>
{
public:
    template <class ContainerType>
    requires concepts::ClassContainer<ContainerType,CoordinateType>
    Cube( const ContainerType& base, 
          CoordinateType length );

protected:
    virtual Bool at( const typename Cube<Dimension,CoordinateType>::point_type& point ) const override;
};

} // namespace boolean


} // namespace cie::csg

#include "CSG/packages/primitives/impl/Cube_impl.hpp"

#endif