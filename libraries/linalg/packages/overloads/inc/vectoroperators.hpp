#ifndef CIE_LINALG_VECTOR_OPERATORS
#define CIE_LINALG_VECTOR_OPERATORS

// --- Utility Includes ---
#include <cieutils/concepts.hpp>

// --- Internal Includes ---
#include "../../types/inc/vectortypes.hpp"
#include "../../types/inc/arraytypes.hpp"


// ---------------------------------------------------------
// VECTOR - SCALAR OPERATORS
// ---------------------------------------------------------

template <cie::concepts::NumericContainer ArrayType, cie::concepts::NumericType ScalarType>
ArrayType operator+( const ArrayType& vector, const ScalarType& scalar );

template <cie::concepts::NumericContainer ArrayType, cie::concepts::NumericType ScalarType>
ArrayType operator+( const ScalarType& scalar, const ArrayType& vector );

template <cie::concepts::NumericContainer ArrayType, cie::concepts::NumericType ScalarType>
ArrayType operator-( const ArrayType& vector, const ScalarType& scalar );

template <cie::concepts::NumericContainer ArrayType, cie::concepts::NumericType ScalarType>
ArrayType operator-( const ScalarType& scalar, const ArrayType& vector );

template <cie::concepts::NumericContainer ArrayType, cie::concepts::NumericType ScalarType>
ArrayType operator*( const ArrayType& vector, const ScalarType& scalar );

template <cie::concepts::NumericContainer ArrayType, cie::concepts::NumericType ScalarType>
ArrayType operator*( const ScalarType& scalar, const ArrayType& vector );

template <cie::concepts::NumericContainer ArrayType, cie::concepts::NumericType ScalarType>
ArrayType operator/( const ArrayType& vector, const ScalarType& scalar );


// ---------------------------------------------------------
// VECTOR - VECTOR OPERATORS
// ---------------------------------------------------------

template <cie::concepts::NumericContainer ArrayType>
ArrayType operator+( const ArrayType& lhs, const ArrayType& rhs );

template <cie::concepts::NumericContainer ArrayType>
ArrayType operator-( const ArrayType& lhs, const ArrayType& rhs );

template <cie::concepts::NumericContainer ArrayType>
typename ArrayType::value_type operator*( const ArrayType& lhs, const ArrayType& rhs );


#include "../impl/vectoroperators_impl.hpp"

#endif