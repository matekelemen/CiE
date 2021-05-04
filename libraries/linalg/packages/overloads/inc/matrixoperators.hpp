#ifndef CIE_LINALG_MATRIX_OPERATORS
#define CIE_LINALG_MATRIX_OPERATORS

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/container_concepts.hpp"

// --- Internal Includes ---
#include "linalg/packages/types/inc/arraytypes.hpp"
#include "linalg/packages/types/inc/vectortypes.hpp"
#include "linalg/packages/types/inc/matrix.hpp"

namespace cie::linalg {

template <concepts::NumericType ValueType, concepts::NumericType ScalarType>
Matrix<ValueType> operator+(const Matrix<ValueType>& matrix, ScalarType scalar);

template <concepts::NumericType ValueType, concepts::NumericType ScalarType>
Matrix<ValueType> operator+(ScalarType scalar, const Matrix<ValueType>& matrix);

template <concepts::NumericType ValueType, concepts::NumericType ScalarType>
Matrix<ValueType> operator-(const Matrix<ValueType>& matrix, ScalarType scalar);

template <concepts::NumericType ValueType>
Matrix<ValueType> operator+(const Matrix<ValueType>& lhs, const Matrix<ValueType>& rhs);

template <concepts::NumericType ValueType>
Matrix<ValueType> operator-(const Matrix<ValueType>& lhs, const Matrix<ValueType>& rhs);

template <concepts::NumericType ValueType, concepts::NumericType ScalarType>
Matrix<ValueType> operator*(const Matrix<ValueType>& matrix, ScalarType scalar);

template <concepts::NumericType ValueType, concepts::NumericType ScalarType>
Matrix<ValueType> operator*(ScalarType scalar, const Matrix<ValueType>& matrix);

template <concepts::NumericType ValueType, concepts::NumericType ScalarType>
Matrix<ValueType> operator/(const Matrix<ValueType>& matrix, ScalarType scalar);

template <concepts::NumericType ValueType, concepts::NumericContainer ContainerType>
ContainerType operator*(const ContainerType& vector, const Matrix<ValueType>& matrix);

template <concepts::NumericType ValueType, concepts::NumericContainer ContainerType>
ContainerType operator*(const Matrix<ValueType>& matrix, const ContainerType& vector);

template <concepts::NumericType ValueType>
Matrix<ValueType> operator*(const Matrix<ValueType>& lhs, const Matrix<ValueType>& rhs);

} // namespace linalg

#include "linalg/packages/overloads/impl/matrixoperators_impl.hpp"

#endif