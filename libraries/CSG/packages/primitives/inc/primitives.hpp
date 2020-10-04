#ifndef CIE_CSG_PRIMITIVES_HPP
#define CIE_CSG_PRIMITIVES_HPP

// --- Internal Includes ---
#include "CSG/packages/primitives/inc/csgobject.hpp"

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/basic_concepts.hpp"

// --- STL Includes ---
#include <tuple>


// ---------------------------------------------------------
// PRIMITIVE CONCEPTS
// ---------------------------------------------------------
namespace cie::concepts {

// Helper for the Primitive concept
namespace detail {

template <class T, class _ = void>
struct is_primitive : std::false_type {};

template <class ...Args>
struct is_primitive_helper {};

template <class T>
struct is_primitive
<
    T,
    std::conditional_t
    <
        false,
        is_primitive_helper
        <
            typename T::coordinate_type,
            typename T::point_type,
            typename T::primitive_constructor_arguments
        >,
        void
    >
> : public std::true_type {};

} // namespace detail

/**
 * A Primitive requires the following type aliases to be valid:
 *  - coordinate_type
 *  - point_type
 *  - primitive_constructor_arguments
*/
template <class T>
concept PrimitiveType
= detail::is_primitive<T>::value;

} // namespace cie::concepts

namespace cie::csg {

// ---------------------------------------------------------
// ABSTRACT PRIMITIVES
// ---------------------------------------------------------

/**
 * Cube template
*/
template <  Size Dimension,
            concepts::NumericType CoordinateType = Double>
class Cube : public CSGTraits<Dimension,CoordinateType>
{
public:
    using primitive_constructor_arguments
        = std::tuple<typename Cube::point_type, typename Cube::coordinate_type>;

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



/**
 * Box template
*/
template <  Size N, 
            concepts::NumericType CoordinateType = Double>
class Box : public CSGTraits<N,CoordinateType>
{
public:
    using primitive_constructor_arguments
        = std::tuple<typename Box::point_type, typename Box::point_type>;

public:
    template <class ContainerType1, class ContainerType2>
    Box(    const ContainerType1& base, 
            const ContainerType2& lengths )
    requires concepts::ClassContainer<ContainerType1,CoordinateType>
                && concepts::ClassContainer<ContainerType2,CoordinateType>;

    const typename Box::point_type& base() const;
    const typename Box::point_type& lengths() const;
    typename Box::point_type& base();
    typename Box::point_type& lengths();

protected:
    typename Box::point_type     _base;
    typename Box::point_type     _lengths;
};


// ---------------------------------------------------------
// SPECIALIZED BOOLEAN PRIMITIVES
// ---------------------------------------------------------
namespace boolean {


/**
 * Cube with point membership test
*/
template <Size N, concepts::NumericType CoordinateType = Double>
class CSGCube :
    public Cube<N,CoordinateType>,
    public CSGObject<N,Bool,CoordinateType>
{
public:
    template <class ContainerType>
    CSGCube(    const ContainerType& base, 
                CoordinateType length )
        requires concepts::ClassContainer<ContainerType,CoordinateType>;

protected:
    virtual Bool at( const typename CSGCube::point_type& point ) const override;
};


/**
 * Box with point membership test
*/
template <Size N, concepts::NumericType CoordinateType = Double>
class CSGBox :
    public Box<N,CoordinateType>,
    public CSGObject<N,Bool,CoordinateType>
{
public:
    template <class ContainerType1, class ContainerType2>
    CSGBox( const ContainerType1& center, 
            const ContainerType2& lengths )
        requires concepts::ClassContainer<ContainerType1,CoordinateType>
                    && concepts::ClassContainer<ContainerType2,CoordinateType>;

protected:
    virtual Bool at( const typename CSGBox::point_type& point ) const override;
};

} // namespace boolean


// ---------------------------------------------------------
// SPECIALIZED NUMERIC PRIMITIVES
// ---------------------------------------------------------
namespace numeric {
// TODO
} // namespace numeric


} // namespace cie::csg

#include "CSG/packages/primitives/impl/primitives_impl.hpp"

#endif