#ifndef LINALG_ERROR_TYPES_HPP
#define LINALG_ERROR_TYPES_HPP

#include <string>
#include <stdexcept>
#include <fstream>

#include "linalghelper.hpp"

namespace linalg {

class MatrixError : public std::runtime_error {
public:
    MatrixError(const std::string& message, const Matrix& matrix);
};

}


#endif