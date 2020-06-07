#ifndef LINALG_QR_FACTORIZATION_HPP
#define LINALG_QR_FACTORIZATION_HPP

#include "../../types/inc/types.hpp"
#include "../../overloads/inc/vectoroperators.hpp"
#include "../../overloads/inc/matrixoperators.hpp"
#include <utility>
#include <memory>

namespace cie {
namespace linalg {

template <class ValueType>
std::pair<MatrixPtr<ValueType>,MatrixPtr<ValueType>> QRFactorization(const Matrix<ValueType>& matrix);

}
}

#include "QRFactorization_impl.hpp"

#endif