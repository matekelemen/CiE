#ifndef LINALG_MATRIX_HPP
#define LINALG_MATRIX_HPP

#include <vector>
#include <iostream>
#include <array>

namespace linalg
{

// Represents a full double vector
using Vector = std::vector<double>;

class Matrix
{
public:
    explicit Matrix( );

    Matrix( size_t size1, size_t size2 );
    Matrix( size_t size1, size_t size2, double value );

    Matrix( const std::vector<double>& rowMajorData, size_t size1 );
    Matrix( const std::vector<Vector>& vectorOfRows );

    size_t size1( ) const;
    size_t size2( ) const;

    std::array<size_t, 2> sizes( ) const;

    double& operator()( size_t i, size_t j );
    double operator()( size_t i, size_t j ) const;

	const std::vector<double>& data() const { return data_; }

private:
    size_t size1_, size2_;
    std::vector<double> data_;
}; // class Matrix

} // namespace linalg

#include "matrix_impl.hpp"

#endif