#include "../inc/errortypes.hpp"

namespace linalg {

MatrixError::MatrixError(const std::string& message, const Matrix& matrix) :
    std::runtime_error(message)
{
    std::ofstream file(message + ".csv");
    if (file.is_open()){
        linalghelper::write(matrix,file);
        file.close();
    }
}

}