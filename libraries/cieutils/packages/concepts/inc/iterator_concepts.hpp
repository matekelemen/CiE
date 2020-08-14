#ifndef CIE_CIEUTILS_ITERATOR_CONCEPTS_HPP
#define CIE_CIEUTILS_ITERATOR_CONCEPTS_HPP

// --- Internal Includes ---
#include "basic_concepts.hpp"

// --- STL Includes ---
#include <iterator>

namespace cie::concepts {


namespace detail {

template<typename T, typename = void>
struct is_iterator
{
   static constexpr bool value = false;
};

template<typename T>
struct is_iterator<T, typename std::enable_if<!std::is_same_v<typename std::iterator_traits<T>::value_type, void>>::type>
{
   static constexpr bool value = true;
};

}


template <class T>
concept IteratorType
= detail::is_iterator<T>::value;


template <class ItType, class ValueType>
concept ClassIterator
=   IteratorType<ItType>
    && std::is_same_v<ValueType, typename std::iterator_traits<ItType>::value_type>;


} // namespace cie::concepts

#endif