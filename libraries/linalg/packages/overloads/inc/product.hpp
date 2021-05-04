#ifndef CIE_LINALG_PRODUCT_HPP
#define CIE_LINALG_PRODUCT_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/container_concepts.hpp"

// --- Internal Includes ---
#include "linalg/packages/types/inc/VectorWrapper.hpp"
#include "linalg/packages/types/inc/MatrixWrapper.hpp"


namespace cie::linalg {


template <class ArrayType, concepts::NumericType NT>
void scale( VectorWrapper<ArrayType>& r_vector, NT coefficient );

template <class MatrixType, concepts::NumericType NT>
void scale( MatrixWrapper<MatrixType>& r_matrix, NT coefficient );

template <concepts::NumericContainer ArrayType>
typename ArrayType::value_type dotProduct( const VectorWrapper<ArrayType>& r_lhs, const VectorWrapper<ArrayType>& r_rhs );

template <class MatrixType, class ArrayType>
VectorWrapper<ArrayType> dotProduct( const MatrixWrapper<MatrixType>& r_lhs, const VectorWrapper<ArrayType>& r_rhs );

template <class ArrayType, class MatrixType>
VectorWrapper<ArrayType> dotProduct( const VectorWrapper<ArrayType>& r_lhs, const MatrixWrapper<MatrixType>& r_rhs );


} // namespace cie::linalg

#include "linalg/packages/overloads/impl/product_impl.hpp"

#endif