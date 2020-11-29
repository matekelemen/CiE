#ifndef CIE_CIEUTILS_BASIC_CONCEPTS_HPP
#define CIE_CIEUTILS_BASIC_CONCEPTS_HPP

// --- STL Includes ---
#include <concepts>
#include <type_traits>
#include <cstdint>

namespace cie {
namespace concepts {


// ---------------------------------------------------------
// STL Extension
// ---------------------------------------------------------

template <class T, class TT>
concept same_as_noqualifiers
= std::is_same_v<T,TT> || std::is_same_v<std::remove_const<T>::type,TT>;

// ---------------------------------------------------------
// TRAITS
// ---------------------------------------------------------
template <class T>
concept CopyConstructible
= std::copy_constructible<T>;


namespace detail {
template<typename T>
struct void_ { typedef void type; };

template<typename T, typename = void>
struct CanBeDerivedFrom {
  static const bool value = false;
};

template<typename T>
struct CanBeDerivedFrom<T, typename void_<int T::*>::type> {
  static const bool value = true;
};
} // namespace detail


template <class T>
concept Deriveable
= detail::CanBeDerivedFrom<T>::value
    && !std::is_integral_v<T>
    && !std::is_floating_point_v<T>;


template <class T>
concept NonDeriveable
= !Deriveable<T>;


// ---------------------------------------------------------
// NUMERICS
// ---------------------------------------------------------
template <class T>
concept Incrementable
= requires ( T instance )
    {
        { ++instance };
        { instance++ };
    };


template <class T>
concept Decrementable
= requires ( T instance )
    {
        { --instance };
        { instance-- };
    };


template <class T>
concept Addable
= requires ( T instance )
    {
        { instance + instance }     -> std::same_as<T>;
        { instance += instance }    -> std::same_as<T&>;
    };


template <class T>
concept Subtractable
= requires ( T instance )
    {
        { instance - instance }     -> std::same_as<T>;
        { instance -= instance }    -> std::same_as<T&>;
    };


template <class T>
concept Multipliable
= requires ( T instance )
    {
        { instance * instance }     -> std::same_as<T>;
        { instance *= instance }    -> std::same_as<T&>;
    };


template <class T>
concept Divisible
= requires ( T instance )
    {
        { instance / instance }     -> std::same_as<T>;
        { instance /= instance }    -> std::same_as<T&>;
    };


template <class T>
concept NumericType
=   Incrementable<T>
    && Decrementable<T>
    && Addable<T>
    && Subtractable<T>
    && Multipliable<T>
    && Divisible<T>;


template <class T>
concept UnsignedInteger
=   std::is_same_v<T,uint8_t>
    || std::is_same_v<T,uint16_t>
    || std::is_same_v<T,uint32_t>
    || std::is_same_v<T,uint64_t>
    || std::is_same_v<T,uint_fast8_t>
    || std::is_same_v<T,uint_fast16_t>
    || std::is_same_v<T,uint_fast32_t>
    || std::is_same_v<T,uint_fast64_t>
    || std::is_same_v<T,uint_least8_t>
    || std::is_same_v<T,uint_least16_t>
    || std::is_same_v<T,uint_least32_t>
    || std::is_same_v<T,uint_least64_t>;


template <class T>
concept SignedInteger
=   std::is_same_v<T,int8_t>
    || std::is_same_v<T,int16_t>
    || std::is_same_v<T,int32_t>
    || std::is_same_v<T,int64_t>
    || std::is_same_v<T,int_fast8_t>
    || std::is_same_v<T,int_fast16_t>
    || std::is_same_v<T,int_fast32_t>
    || std::is_same_v<T,int_fast64_t>
    || std::is_same_v<T,int_least8_t>
    || std::is_same_v<T,int_least16_t>
    || std::is_same_v<T,int_least32_t>
    || std::is_same_v<T,int_least64_t>;


template <class T>
concept Integer
= UnsignedInteger<T> || SignedInteger<T>;


// ---------------------------------------------------------
// CONVERSION
// ---------------------------------------------------------
template <class SourceType, class TargetType>
concept ConvertibleTo
= requires (SourceType instance)
{
    { TargetType(instance) };
};


// ---------------------------------------------------------
// POINTERS
// ---------------------------------------------------------
template <class T>
concept Dereferencable
= requires ( T instance )
{
    { *instance };
    { instance.operator->() };
};


template <class T>
concept RawPointer
= std::is_pointer_v<T>;


template <class T>
concept NonRawPointer
= !std::is_pointer_v<T>;


template <class T>
concept Pointer
= RawPointer<T>
  || Dereferencable<T>;


template <class T>
concept NonPointer
= !Pointer<T>;


template <class PointerType, class ValueType>
concept ClassPointer
= Pointer<PointerType>
  && requires ( PointerType instance )
{
    { *instance } -> std::same_as<ValueType&>;
};


template <class PointerType, class ValueType>
concept ClassRawPointer
= RawPointer<PointerType>
  && ClassPointer<PointerType, ValueType>;


// ---------------------------------------------------------
// POLYMORPHISM
// ---------------------------------------------------------
template <class DerivedType, class BaseType>
concept DerivedFrom
= std::derived_from<DerivedType,BaseType>;


}
}

#endif