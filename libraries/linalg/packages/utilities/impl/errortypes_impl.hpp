#ifndef CIE_LINALG_ERROR_TYPES_IMPL_HPP
#define CIE_LINALG_ERROR_TYPES_IMPL_HPP

// --- Internal Includes ---
#include "cmake_variables.hpp"

// --- STL Includes ---
#include <fstream>
#include <iostream>

namespace cie::linalg {

template <class MatrixType>
MatrixError<MatrixType>::MatrixError(const String& message, const MatrixType& matrix) :
    Exception( message + "\nmatrix dumped to " + DEBUG_OUTPUT_PATH + "/" + message + ".csv" )
{
    std::ofstream file( DEBUG_OUTPUT_PATH + "/" + message + ".csv");
    if (file.is_open()){
        linalghelper::write(matrix,file);
        file.close();
    }
}

} // namespace cie::linalg

#endif