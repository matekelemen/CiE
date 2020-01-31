#ifndef LINALG_MATRIX_TYPES_HPP
#define LINALG_MATRIX_TYPES_HPP

#include "matrix.hpp"
#include "sparse.hpp"
#include <memory>

namespace cie {

using MatrixPtr = std::shared_ptr<linalg::Matrix>;

}

#endif