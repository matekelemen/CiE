#ifndef CIE_CIEUTILS_BASIC_CONCEPTS_HPP
#define CIE_CIEUTILS_BASIC_CONCEPTS_HPP

// --- STD Includes ---
#include <concepts>
#include <type_traits>

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