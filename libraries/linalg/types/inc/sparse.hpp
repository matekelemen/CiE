#ifndef LINALG_SPARSE_HPP
#define LINALG_SPARSE_HPP

#include "linalg.hpp"

#include <vector>
#include <tuple>
#include <cstddef>

namespace cie
{
namespace linalg
{

template<typename IndexType>
class CompressedSparseRowMatrix
{
public:
    explicit CompressedSparseRowMatrix( );

    void allocate( const std::vector<std::vector<size_t>>& locationMaps );

    size_t size1( ) const;
    size_t size2( ) const;
    IndexType nnz( ) const;

    double operator()( size_t i, size_t j ) const;
    std::vector<double> operator*( const std::vector<double>& vector );

    void scatter( const linalg::Matrix& elementMatrix,
                  const std::vector<size_t>& locationMap );

    //! Obtain sparse data structure and set matrix to zero
    std::tuple<IndexType*, IndexType*, double*, size_t> release( );

private:
    IndexType* indices_;
    IndexType* indptr_;
    double* data_;

    size_t size1_, size2_;

    void cleanup( );

public:
    ~CompressedSparseRowMatrix( );

    // disable copying
    CompressedSparseRowMatrix( const CompressedSparseRowMatrix& ) = delete;
    CompressedSparseRowMatrix& operator=( const CompressedSparseRowMatrix& ) = delete;
};

} // namespace splinekernel
} // namespace cie

#include "sparse_impl.hpp"

#endif // CIE_SPARSE_HPP
