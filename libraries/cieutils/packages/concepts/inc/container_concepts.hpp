#ifndef CIE_CIEUTILS_CONTAINER_CONCEPTS_HPP
#define CIE_CIEUTILS_CONTAINER_CONCEPTS_HPP

// --- Internal Includes ---
#include "cieutils/packages/concepts/inc/iterator_concepts.hpp"


namespace cie::concepts {


namespace detail {
template<typename T, typename _ = void>
struct is_container : std::false_type {};

template<typename... Ts>
struct is_container_helper {};

template<typename T>
struct is_container<
        T,
        std::conditional_t<
            false,
            is_container_helper<
                typename T::value_type,
                typename T::size_type,
                typename T::iterator,
                decltype(std::declval<T>().size()),
                decltype(std::declval<T>().begin()),
                decltype(std::declval<T>().end())
                >,
            void
            >
        > : public std::true_type {};
} // namespace detail


template <class T>
concept STLContainer
= detail::is_container<T>::value;


template <class ContainerType, class ValueType>
concept ClassContainer
= STLContainer<typename std::remove_reference<ContainerType>::type>
  && std::is_same_v<typename std::remove_reference<ContainerType>::type::value_type, ValueType>;


template <class ContainerType>
concept NumericContainer
= STLContainer<typename std::remove_reference<ContainerType>::type>
  && NumericType<typename std::remove_reference<ContainerType>::type::value_type>;


template <class ContainerType>
concept PointerContainer
= STLContainer<typename std::remove_reference<ContainerType>::type>
  && Pointer<typename std::remove_reference<ContainerType>::type::value_type>;


template <class ContainerType>
concept NonPointerContainer
= STLContainer<typename std::remove_reference<ContainerType>::type>
  && !PointerContainer<ContainerType>;


template <class ContainerType>
concept IteratorContainer
= STLContainer<typename std::remove_reference<ContainerType>::type>
  && IteratorType<typename std::remove_reference<ContainerType>::type::value_type>;


template <class ContainerType, class ValueType>
concept ClassPointerContainer
= STLContainer<typename std::remove_reference<ContainerType>::type>
  && ClassPointer<typename std::remove_reference<ContainerType>::type::value_type,ValueType>;


template <class ContainerType, class InterfaceType>
concept InterfaceContainer
= STLContainer<typename std::remove_reference<ContainerType>::type>
  && Pointer<typename std::remove_reference<ContainerType>::type::value_type>
  && DerivedFrom<typename std::pointer_traits<typename std::remove_reference<ContainerType>::type::value_type>::element_type,InterfaceType>;


// ---------------------------------------------------------
// SPECIALIZED STL CONTAINERS
// ---------------------------------------------------------

template <class ContainerType>
concept ResizableContainer
= STLContainer<ContainerType>
  && requires ( ContainerType instance )
{
    { instance.resize(1) };
};


template <class ContainerType>
concept ReservableContainer
= STLContainer<ContainerType>
  && requires ( ContainerType instance )
{
    { instance.reserve(1) };
};


// TODO: improve definition
template <class ContainerType>
concept FixedSizeContainer
=   STLContainer<ContainerType>
    && !ResizableContainer<ContainerType>;


template <class ContainerType>
concept STLArray
= FixedSizeContainer<ContainerType>;


} // namespace cie::concepts

#endif