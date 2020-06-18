#ifndef CIE_CIEUTILS_CONTAINER_CONCEPTS_HPP
#define CIE_CIEUTILS_CONTAINER_CONCEPTS_HPP

// --- Internal Includes ---
#include "iterator_concepts.hpp"


namespace cie {
namespace concepts {


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
                typename T::const_iterator,
                decltype(std::declval<T>().size()),
                decltype(std::declval<T>().begin()),
                decltype(std::declval<T>().end()),
                decltype(std::declval<T>().cbegin()),
                decltype(std::declval<T>().cend())
                >,
            void
            >
        > : public std::true_type {};
}


template <class T>
concept STLContainer
= detail::is_container<T>::value;


template <class ContainerType, class ValueType>
concept ClassContainer
=   STLContainer<ContainerType>
    && std::is_same_v<typename ContainerType::value_type, ValueType>;


template <class ContainerType>
concept NumericContainer
=   STLContainer<ContainerType>
    && NumericType<typename ContainerType::value_type>;


template <class ContainerType>
concept PointerContainer
=   STLContainer<ContainerType>
    && PointerType<typename ContainerType::value_type>;


template <class ContainerType>
concept NonPointerContainer
=   STLContainer<ContainerType>
    && NonPointerType<typename ContainerType::value_type>;


template <class ContainerType>
concept IteratorContainer
=   STLContainer<ContainerType>
    && IteratorType<typename ContainerType::value_type>;


}
}

#endif