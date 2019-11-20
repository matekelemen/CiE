#include "../inc/matrixoperators.hpp"

namespace cie {
namespace linalg {

Matrix operator+(const Matrix& matrix, double scalar)
{
    DoubleVector rowMajorData(matrix.size1() * matrix.size2());
    std::transform( 
        matrix.data().begin(), 
        matrix.data().end(), 
        rowMajorData.begin(), 
        [scalar](auto matrixIt) -> double
            {return matrixIt + scalar;}
        );
    
    return Matrix(rowMajorData, matrix.size1());
}


Matrix operator+(double scalar, const Matrix& matrix)
{
    return matrix + scalar;
}


Matrix operator-(const Matrix& matrix, double scalar) 
{
    return matrix + (-scalar);
}


Matrix operator+(const Matrix& lhs, const Matrix& rhs)
{
    // Check sizes
    if (lhs.size1() != rhs.size1()  ||  lhs.size2() != rhs.size2())
        throw std::runtime_error("Matrix size mismatch!");
    
    DoubleVector rowMajorData(lhs.size1()*lhs.size2());
    std::transform(
        lhs.data().begin(),
        lhs.data().end(),
        rhs.data().begin(),
        rowMajorData.begin(),
        [](auto lhsIt, auto rhsIt) -> double
            {return lhsIt + rhsIt;}
    );

    return Matrix(rowMajorData,lhs.size1());
}


Matrix operator-(const Matrix& lhs, const Matrix& rhs)
{
    // Check sizes
    if (lhs.size1() != rhs.size1()  ||  lhs.size2() != rhs.size2())
        throw std::runtime_error("Matrix size mismatch!");
    
    DoubleVector rowMajorData(lhs.size1()*lhs.size2());
    std::transform(
        lhs.data().begin(),
        lhs.data().end(),
        rhs.data().begin(),
        rowMajorData.begin(),
        [](auto lhsIt, auto rhsIt) -> double
            {return lhsIt - rhsIt;}
    );

    return Matrix(rowMajorData,lhs.size1());
}


Matrix operator*(const Matrix& matrix, double scalar)
{
    DoubleVector rowMajorData(matrix.size1() * matrix.size2());
    std::transform( 
        matrix.data().begin(), 
        matrix.data().end(), 
        rowMajorData.begin(), 
        [scalar](auto matrixIt) -> double
            {return matrixIt * scalar;}
        );
    
    return Matrix(rowMajorData, matrix.size1());
}


Matrix operator*(double scalar, const Matrix& matrix)
{
    return matrix * scalar;
}


Matrix operator/(const Matrix& matrix, double scalar)
{
    if (std::abs(scalar)<1e-16)
        throw std::runtime_error("Division by 0!");
    return matrix * (1/scalar);
}


DoubleVector operator*(const DoubleVector& vector, const Matrix& matrix)
{
    // Check size compatibility
    if (vector.size() != matrix.size1())
        throw std::runtime_error(
            "Cannot multiply vector of size " + 
            std::to_string(vector.size()) +
            " by matrix of size " + 
            std::to_string(matrix.size1()) + "x" + std::to_string(matrix.size2()) 
            );
    
    DoubleVector result(matrix.size2(),0.0);
    for (size_t j=0; j<matrix.size2(); ++j){
        for (size_t i=0; i<matrix.size1(); ++i) 
            result[j] += vector[i] * matrix(i,j);
    }

    return result;
}


DoubleVector operator*(const Matrix& matrix, const DoubleVector& vector)
{
    // Check size compatibility
    if (vector.size() != matrix.size2())
        throw std::runtime_error(
            "Cannot multiply matrix of size " + 
            std::to_string(matrix.size1()) + "x" + std::to_string(matrix.size2()) +
            " by vector of size " + 
            std::to_string(vector.size())
            );
    
    DoubleVector result(matrix.size1(),0.0);
    for (size_t i=0; i<matrix.size1(); ++i){
        for (size_t j=0; j<matrix.size2(); ++j) 
            result[i] += matrix(i,j) * vector[j];
    }

    return result;
}


Matrix operator*(const Matrix& lhs, const Matrix& rhs)
{
    // Check size compatibility
    if (lhs.size2() != rhs.size1())
        throw std::runtime_error(
            "Cannot multiply matrix of size " + 
            std::to_string(lhs.size1()) + "x" + std::to_string(lhs.size2()) +
            " by matrix of size " + 
            std::to_string(rhs.size1()) + "x" + std::to_string(rhs.size2())
            );

    Matrix result(lhs.size1(), rhs.size2(), 0.0);

    for (size_t i=0; i<lhs.size1(); ++i){
        for (size_t j=0; j<rhs.size2(); ++j){

            for (size_t k=0; k<lhs.size2(); ++k){
                result(i,j) += lhs(i,k) * rhs(k,j);
            }

        }
    }

    return result;
}


}
}