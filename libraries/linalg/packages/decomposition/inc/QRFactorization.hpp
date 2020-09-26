#ifndef LINALG_QR_FACTORIZATION_HPP
#define LINALG_QR_FACTORIZATION_HPP

// --- Internal Includes ---
#include "linalg/packages/types/inc/matrix.hpp"
#include "linalg/packages/overloads/inc/vectoroperators.hpp"
#include "linalg/packages/overloads/inc/matrixoperators.hpp"

// --- STL Includes ---
#include <utility>
#include <memory>

namespace cie::linalg {

template <class ValueType>
std::pair<MatrixPtr<ValueType>,MatrixPtr<ValueType>> QRFactorization(const Matrix<ValueType>& matrix);

} // namespace cie::linalg

#include "linalg/packages/decomposition/impl/QRFactorization_impl.hpp"

#endif