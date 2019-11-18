#ifndef LINALG_MATRIX_OPERATORS_IMPL_HPP
#define LINALG_MATRIX_OPERATORS_IMPL_HPP

namespace linalg {

template <size_t N>
DoubleArray<N> operator*(const DoubleArray<N>& vector, const Matrix& matrix)
{
    // Check size compatibility
    if (vector.size() != matrix.size1())
        throw std::runtime_error(
            "Cannot multiply vector of size " + 
            std::to_string(vector.size()) +
            " by matrix of size " + 
            std::to_string(matrix.size1()) + "x" + std::to_string(matrix.size2()) 
            );
    
    DoubleArray<N> result;
    result.fill(0.0);
    for (size_t j=0; j<matrix.size2(); ++j){
        for (size_t i=0; i<matrix.size1(); ++i) 
            result[j] += vector[i] * matrix(i,j);
    }

    return result;
}


template <size_t N>
DoubleArray<N> operator*(const Matrix& matrix, const DoubleArray<N>& vector)
{
    // Check size compatibility
    if (vector.size() != matrix.size2())
        throw std::runtime_error(
            "Cannot multiply matrix of size " + 
            std::to_string(matrix.size1()) + "x" + std::to_string(matrix.size2()) +
            " by vector of size " + 
            std::to_string(vector.size())
            );
    
    DoubleArray<N> result;
    result.fill(0.0);
    for (size_t i=0; i<matrix.size1(); ++i){
        for (size_t j=0; j<matrix.size2(); ++j) 
            result[i] += matrix(i,j) * vector[j];
    }

    return result;
}

}

#endif