#ifndef CIE_CIEUTILS_BASIC_CONCEPTS_HPP
#define CIE_CIEUTILS_BASIC_CONCEPTS_HPP

// --- STL Includes ---
#include <concepts>
#include <type_traits>
#include <cstdint>

namespace cie {
namespace concepts {


// ---------------------------------------------------------
// TRAITS
// ---------------------------------------------------------
template <class T>
concept CopyConstructible
= std::copy_constructible<T>;


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
concept Dividable
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
    && Dividable<T>;


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
concept PointerType
= std::is_pointer_v<T>;


template <class T>
concept NonPointerType
= !std::is_pointer_v<T>;


template <class PtrType, class ValueType>
concept ClassPointer
=   PointerType<PtrType>
    && requires( PtrType ptr )
        {
            { *ptr } -> std::same_as<ValueType&>;
        };


// ---------------------------------------------------------
// POLYMORPHISM
// ---------------------------------------------------------
template <class DerivedType, class BaseType>
concept DerivedFrom
= std::derived_from<DerivedType,BaseType>;


}
}

#endif