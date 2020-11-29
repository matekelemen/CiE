#ifndef CIE_CSG_PRIMITIVES_CONCEPTS_HPP
#define CIE_CSG_PRIMITIVES_CONCEPTS_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/container_concepts.hpp"

// --- Internal Includes ---
#include "CSG/packages/primitives/inc/Primitive.hpp"


namespace cie::concepts {


template <class T>
concept PrimitivePtr
= Pointer<T>
  && Primitive<typename std::pointer_traits<T>::element_type>;


template <class T>
concept PrimitiveContainer
= STLContainer<T> && Primitive<typename T::value_type>;


template <class T>
concept PrimitivePtrContainer
= STLContainer<T> && PrimitivePtr<typename T::value_type>;


} // namespace cie::concepts

#endif