#ifndef CIE_MESH_KERNEL_MARCHING_PRIMITIVES_CONCEPTS_HPP
#define CIE_MESH_KERNEL_MARCHING_PRIMITIVES_CONCEPTS_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/basic_concepts.hpp"

// --- Internal Includes ---
#include "meshkernel/packages/traits/inc/MeshTraits.hpp"


namespace cie::concepts {


namespace detail {

template <class T, class _ = void>
struct isMarchingPrimitives : std::false_type {};

template <class ...Args>
struct isMarchingPrimitivesHelper {};

template <class T>
struct isMarchingPrimitives
<
    T,
    std::conditional_t
    <
        false,
        isMarchingPrimitivesHelper
        <
            typename T::target_type
            ,typename T::target_ptr
            ,typename T::edge_type
            ,typename T::output_arguments
            ,typename T::output_functor
            ,typename T::primitive_type
        >,
        void
    >
> : public std::true_type {};

} // namespace detail


template <class T>
concept MarchingPrimitives
= detail::isMarchingPrimitives<T>::value
  && MeshObject<T>
  && requires ( T instance )
{
    { instance.execute() };
};


} // namespace cie::concepts


#endif