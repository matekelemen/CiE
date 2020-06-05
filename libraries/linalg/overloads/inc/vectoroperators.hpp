#ifndef LINALG_VECTOR_OPERATORS
#define LINALG_VECTOR_OPERATORS

// --- Utility Includes ---
#include "cieutils/concepts.hpp"

// --- Internal Includes ---
#include "../../types/inc/matrix.hpp"
#include "../../types/inc/vectortypes.hpp"
#include "../../types/inc/arraytypes.hpp"

namespace cie {

// ---------------------------------------------------------
// VECTOR - SCALAR OPERATORS
// ---------------------------------------------------------
template <concepts::NumericContainer ArrayType, concepts::NumericType ScalarType>
ArrayType operator+( const ArrayType& vector, const ScalarType& scalar );

template <concepts::NumericContainer ArrayType, concepts::NumericType ScalarType>
ArrayType operator+( const ScalarType& scalar, const ArrayType& vector );

template <concepts::NumericContainer ArrayType, concepts::NumericType ScalarType>
ArrayType operator-( const ArrayType& vector, const ScalarType& scalar );

template <concepts::NumericContainer ArrayType, concepts::NumericType ScalarType>
ArrayType operator-( const ScalarType& scalar, const ArrayType& vector );

template <concepts::NumericContainer ArrayType, concepts::NumericType ScalarType>
ArrayType operator*( const ArrayType& vector, const ScalarType& scalar );

template <concepts::NumericContainer ArrayType, concepts::NumericType ScalarType>
ArrayType operator*( const ScalarType& scalar, const ArrayType& vector );

template <concepts::NumericContainer ArrayType, concepts::NumericType ScalarType>
ArrayType operator/( const ArrayType& vector, const ScalarType& scalar );


// ---------------------------------------------------------
// VECTOR - VECTOR OPERATORS
// ---------------------------------------------------------
template <concepts::NumericContainer ArrayType>
ArrayType operator+( const ArrayType& lhs, const ArrayType& rhs );

template <concepts::NumericContainer ArrayType>
ArrayType operator-( const ArrayType& lhs, const ArrayType& rhs );

template <concepts::NumericContainer ArrayType>
typename ArrayType::value_type operator*( const ArrayType& lhs, const ArrayType& rhs );

}

#include "vectoroperators_impl.hpp"

#endif