#ifndef CIE_LINALG_SPARSE_HPP
#define CIE_LINALG_SPARSE_HPP

// --- Linalg Includes ---
#include "linalg/types.hpp"

// --- Utility Includes ---
#include <cieutils/types.hpp>
#include <cieutils/concepts.hpp>

// --- STD Includes ---
#include <vector>
#include <tuple>
#include <cstddef>

namespace cie::linalg {

template<concepts::NumericType IndexType, concepts::NumericType ValueType = Double>
class CompressedSparseRowMatrix
{
public:
    typedef IndexType   index_type;
    typedef ValueType   value_type;

    explicit CompressedSparseRowMatrix( );

    void allocate( const std::vector<std::vector<Size>>& locationMaps );

    Size size1( ) const;
    Size size2( ) const;
    IndexType nnz( ) const;

    ValueType operator()( Size i, Size j ) const;
    std::vector<ValueType> operator*( const std::vector<ValueType>& vector );

    void scatter( const linalg::Matrix<ValueType>& elementMatrix,
                  const std::vector<Size>& locationMap );

    //! Obtain sparse data structure and set matrix to zero
    std::tuple<IndexType*, IndexType*, ValueType*, Size> release( );

private:
    IndexType* indices_;
    IndexType* indptr_;
    ValueType* data_;

    Size size1_, size2_;

    void cleanup( );

public:
    ~CompressedSparseRowMatrix( );

    // disable copying
    CompressedSparseRowMatrix( const CompressedSparseRowMatrix& ) = delete;
    CompressedSparseRowMatrix& operator=( const CompressedSparseRowMatrix& ) = delete;
};

}

#include "sparse_impl.hpp"

#endif // CIE_SPARSE_HPP
