#ifndef CIE_CSG_PRIMITIVES_PRIMITIVE_HPP
#define CIE_CSG_PRIMITIVES_PRIMITIVE_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/basic_concepts.hpp"

// --- Internal Includes ---
#include "CSG/packages/primitives/inc/csgobject.hpp"


// ---------------------------------------------------------
// PRIMITIVE INTERFACE
// ---------------------------------------------------------
namespace cie::csg {


template < Size Dimension,
           concepts::NumericType CoordinateType = Double >
class AbsPrimitive : public CSGTraits<Dimension,CoordinateType>
{
public:
    virtual Bool isDegenerate() const = 0;
};


} // namespace cie::csg




// ---------------------------------------------------------
// PRIMITIVE CONCEPT
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


template <class T>
concept hasIsDegenerate
= requires ( T instance )
{
    { instance.isDegenerate() } -> std::same_as<Bool>;
};

} // namespace detail

/**
 * A Primitive requires the following type aliases to be valid:
 *  - coordinate_type
 *  - point_type
 *  - primitive_constructor_arguments
*/
template <class T>
concept Primitive
= detail::is_primitive<T>::value
    && detail::hasIsDegenerate<T>;

} // namespace cie::concepts


#endif