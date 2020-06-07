#ifndef CIE_LINALG_ERROR_TYPES_IMPL_HPP
#define CIE_LINALG_ERROR_TYPES_IMPL_HPP

// --- STD Includes ---
#include <fstream>
#include <iostream>

namespace cie {
namespace linalg {

template <class MatrixType>
MatrixError<MatrixType>::MatrixError(const String& message, const MatrixType& matrix) :
    Exception( message + "\nmatrix dumped to " + message + ".csv" )
{
    std::ofstream file(message + ".csv");
    if (file.is_open()){
        linalghelper::write(matrix,file);
        file.close();
    }
}

}
}

#endif