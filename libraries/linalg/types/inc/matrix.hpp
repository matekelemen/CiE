#ifndef LINALG_MATRIX_HPP
#define LINALG_MATRIX_HPP

#include "vectortypes.hpp"
#include <vector>
#include <iostream>
#include <array>

namespace linalg
{

class Matrix
{
public:
    explicit Matrix( );

    Matrix( size_t size1, size_t size2 );
    Matrix( size_t size1, size_t size2, double value );

    Matrix( const std::vector<double>& rowMajorData, size_t size1 );
    Matrix( const std::vector<DoubleVector>& vectorOfRows );

    size_t size1( ) const;
    size_t size2( ) const;
    size_t size() const;

    std::array<size_t, 2> sizes( ) const;

    double& operator()( size_t i, size_t j );
    double operator()( size_t i, size_t j ) const;
    double& operator[]( size_t i );
    double operator[]( size_t i ) const;

	const std::vector<double>& data() const { return data_; }

    void transpose();

private:
    size_t size1_, size2_;
    std::vector<double> data_;
    bool _transpose;
}; // class Matrix

} // namespace linalg

#include "matrix_impl.hpp"

#endif