#ifndef LINALG_QR_FACTORIZATION_HPP
#define LINALG_QR_FACTORIZATION_HPP

// --- Internal Includes ---
#include "../../types/inc/matrix.hpp"
#include "../../overloads/inc/vectoroperators.hpp"
#include "../../overloads/inc/matrixoperators.hpp"

// --- STL Includes ---
#include <utility>
#include <memory>

namespace cie::linalg {

template <class ValueType>
std::pair<MatrixPtr<ValueType>,MatrixPtr<ValueType>> QRFactorization(const Matrix<ValueType>& matrix);

}

#include "QRFactorization_impl.hpp"

#endif