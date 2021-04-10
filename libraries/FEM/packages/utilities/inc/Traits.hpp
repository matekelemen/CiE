#ifndef CIE_FEM_UTILITIES_TRAITS_HPP
#define CIE_FEM_UTILITIES_TRAITS_HPP

// --- Internal Includes ---
#include "FEM/packages/utilities/inc/kernel.hpp"


namespace cie::fem {


namespace detail {
template <class T, class _ = void>
struct WithTraits : std::false_type {};

template <class ...Args>
struct WithTraitsHelper {};

template <class T>
struct WithTraits<
    T,
    std::conditional_t<
        false,
        WithTraitsHelper<
            typename T::number_type,
            typename T::point_type
        >,
        void
    >
> : public std::true_type {};

template <class T>
concept HasTraits
= WithTraits<T>::value;
} // namespace detail


/// Invalid general Traits template
template <class ...Args>
struct Traits : public Kernel<void,0>
{};


template <Size Dimension, concepts::NumericType NT>
struct Traits<Dimension,NT> : public Kernel<Dimension,NT>
{};


template <detail::HasTraits T>
struct Traits<T> : public Kernel<T::dimension, typename T::number_type>;


} // namespace cie::fem


#endif