#ifndef LINALG_QR_FACTORIZATION_IMPL_HPP
#define LINALG_QR_FACTORIZATION_IMPL_HPP

// --- Internal Includes ---
#include "linalg/packages/decomposition/inc/QRFactorization.hpp"
#include "linalg/packages/types/inc/typeoperations.hpp"

namespace cie::linalg {


template <class ValueType>
std::pair<MatrixPtr<ValueType>,MatrixPtr<ValueType>> QRFactorization(const Matrix<ValueType>& matrix)
{
    auto Q = std::make_shared<Matrix<ValueType>>( matrix.size1(), matrix.size2(), 0.0 );
    auto R = std::make_shared<Matrix<ValueType>>( matrix.size2(), matrix.size2(), 0.0 );

    std::vector<ValueType> v(matrix.size1());
    ValueType magnitude;

    for (size_t j = 0; j < matrix.size2(); ++j)
    {

        for (size_t k=0; k<matrix.size1(); ++k)
            v[k] = matrix(k,j);

        for (size_t i = 0; i < (j>0 ? j-1 : 0); ++i)
        {
            for (size_t k = 0; k < matrix.size1(); ++k)
                R->operator()(i,j)  += v[k] * Q->operator()(k,i);

            for (size_t k = 0; k < matrix.size1(); ++k)
                v[k] -= R->operator()(i,j)*Q->operator()(k,i);
        }

        magnitude = norm<std::vector<ValueType>>(v);
        for (size_t k=0; k<matrix.size1(); ++k)
            Q->operator()(k,j) = v[k]/magnitude;

        R->operator()(j,j) = magnitude;
    }

    return std::make_pair( Q,R );
}


} // namespace cie::linalg

#endif