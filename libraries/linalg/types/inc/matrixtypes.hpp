#ifndef LINALG_MATRIX_TYPES_HPP
#define LINALG_MATRIX_TYPES_HPP

// --- Internal Includes ---
#include "matrix.hpp"
#include "sparse.hpp"

// --- STL Includes ---
#include <memory>

namespace cie {

template <class ValueType>
using MatrixPtr = std::shared_ptr<linalg::Matrix<ValueType>>;

}

#endif