#include "../inc/QRFactorization.hpp"

#include "../../types/inc/typeoperations.hpp"

namespace cie {
namespace linalg {


std::pair<MatrixPtr, MatrixPtr> QRFactorization(const Matrix& matrix)
{
    Matrix Q( matrix.size1(), matrix.size2(), 0.0 );
    Matrix R( matrix.size2(), matrix.size2(), 0.0 );

    DoubleVector v(matrix.size1());
    double magnitude;

    for (size_t j = 0; j < matrix.size2(); ++j)
    {

        for (size_t k=0; k<matrix.size1(); ++k)
            v[k] = matrix(k,j);

        for (size_t i = 0; i < (j>0 ? j-1 : 0); ++i)
        {
            for (size_t k = 0; k < matrix.size1(); ++k)
                R(i,j)  += v[k] * Q(k,i);

            for (size_t k = 0; k < matrix.size1(); ++k)
                v[k] -= R(i,j)*Q(k,i);
        }

        magnitude = norm<DoubleVector>(v);
        for (size_t k=0; k<matrix.size1(); ++k)
            Q(k,j) = v[k]/magnitude;

        R(j,j) = magnitude;
    }

    return std::make_pair<MatrixPtr,MatrixPtr>( std::make_shared<Matrix>(Q), std::make_shared<Matrix>(R) );
}


}
}