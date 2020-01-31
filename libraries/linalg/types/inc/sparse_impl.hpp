#ifndef LINALG_SPARSE_IMPL_HPP
#define LINALG_SPARSE_IMPL_HPP

#include <algorithm>
#include <numeric>

namespace cie
{
namespace linalg
{

template<typename IndexType>
CompressedSparseRowMatrix<IndexType>::CompressedSparseRowMatrix( ) :
    indices_( nullptr ), indptr_( nullptr ), data_( nullptr ),
    size1_( 0 ), size2_( 0 )
{

}

template<typename IndexType>
std::tuple<IndexType*, IndexType*, double*, size_t> CompressedSparseRowMatrix<IndexType>::release( )
{
    std::tuple<IndexType*, IndexType*, double*, size_t> sparseDataStructure { indices_, indptr_, data_, size1_ };

    indices_ = nullptr;
    indptr_ = nullptr;
    data_ = nullptr;

    size1_ = 0;
    size2_ = 0;

    return sparseDataStructure;
}

template<typename IndexType>
IndexType CompressedSparseRowMatrix<IndexType>::nnz( ) const
{
    return indptr_[size1_];
}

template<typename IndexType>
size_t CompressedSparseRowMatrix<IndexType>::size1( ) const
{
    return size1_;
}

template<typename IndexType>
size_t CompressedSparseRowMatrix<IndexType>::size2( ) const
{
    return size2_;
}

template<typename IndexType>
void CompressedSparseRowMatrix<IndexType>::cleanup( )
{
    delete[] indices_;
    delete[] indptr_;
    delete[] data_;

    indices_ = nullptr;
    indptr_ = nullptr;
    data_ = nullptr;

    size1_ = 0;
    size2_ = 0;
}

template<typename IndexType>
double CompressedSparseRowMatrix<IndexType>::operator ()( size_t i,
                                               size_t j ) const
{
    auto result = std::find( indices_ + indptr_[i], indices_ + indptr_[i + 1], j );

    if( result )
    {
        return *result;
    }

    return 0.0;
}

template<typename IndexType>
std::vector<double> CompressedSparseRowMatrix<IndexType>::operator*( const std::vector<double>& vector )
{
    if (vector.size() != size2_)
        throw std::runtime_error("Inconsistent sizes in matrix vector multiplication.");

    std::vector<double> result( size1_, 0.0 );

    for( size_t i = 0; i < size1_; ++i )
    {
        for( IndexType j = indptr_[i]; j < indptr_[i + 1]; ++j )
        {
            result[i] += data_[j] * vector[indices_[j]];
        }
    }

    return result;
}

template<typename IndexType>
CompressedSparseRowMatrix<IndexType>::~CompressedSparseRowMatrix( )
{
    cleanup( );
}



template<typename IndexType>
void CompressedSparseRowMatrix<IndexType>::allocate( const std::vector<std::vector<size_t>>& locationMaps )
{
    cleanup( );

    size_t numberOfElements = locationMaps.size( );
    size_t globalNumberOfDofs = 0;

    for( const auto& locationMap : locationMaps )
    {
        size_t maxElementIndex = *std::max_element( locationMap.begin( ), locationMap.end( ) ) + 1;

        if( maxElementIndex > globalNumberOfDofs )
        {
            globalNumberOfDofs = maxElementIndex;
        }
    }

    std::vector<std::vector<size_t>> dofToElementCoupling( globalNumberOfDofs );

    for( size_t iElement = 0; iElement < numberOfElements; ++iElement )
    {
        for( size_t dofIndex : locationMaps[iElement] )
        {
            dofToElementCoupling[dofIndex].push_back( iElement );
        }
    }

    std::vector<std::vector<size_t>> dofToDofCoupling( globalNumberOfDofs );
    std::vector<size_t> tmp;

    for( size_t iDof = 0; iDof < globalNumberOfDofs; ++iDof )
    {
        if (dofToElementCoupling[iDof].size( ) <= 0)
            throw std::runtime_error("Found dof without connected element!");

        tmp.resize( 0 );

        for( size_t iElement = 0; iElement < dofToElementCoupling[iDof].size( ); ++iElement )
        {
            const auto& locationMap = locationMaps[dofToElementCoupling[iDof][iElement]];

            tmp.insert( tmp.end( ), locationMap.begin( ), locationMap.end( ) );
        }

        std::sort( tmp.begin( ), tmp.end( ) );
        auto end = std::unique( tmp.begin( ), tmp.end( ) );

        dofToDofCoupling[iDof].insert( dofToDofCoupling[iDof].end( ), tmp.begin( ), end );
    }

    std::vector<std::vector<size_t>>{ }.swap( dofToElementCoupling );

    size1_ = globalNumberOfDofs;
    size2_ = globalNumberOfDofs;

    indptr_ = new IndexType[size1_ + 1];
    indptr_[0] = 0;

    auto vectorSizePredicate = []( const std::vector<size_t>& dofs ) { return static_cast<IndexType>( dofs.size( ) ); };

    // Fill indptr with cumulative sum of the vector sizes of dofToDofCoupling
    std::transform( dofToDofCoupling.begin( ), dofToDofCoupling.end( ), indptr_ + 1, vectorSizePredicate );
    std::partial_sum( indptr_, indptr_ + size1_ + 1, indptr_ );

    IndexType nnz = indptr_[size1_];

    indices_ = new IndexType[nnz];

    // Concatenate dof indices
    IndexType* currentRow = indices_;

    for( const std::vector<size_t>& dofs : dofToDofCoupling )
    {
        std::copy( dofs.begin( ), dofs.end( ), currentRow );

        currentRow += dofs.size( );
    }

    std::vector<std::vector<size_t>>{ }.swap( dofToDofCoupling );

    // Resize data
    data_ = new double[nnz];

    std::fill( data_, data_ + nnz, 0.0 );
}

template<typename IndexType>
void CompressedSparseRowMatrix<IndexType>::scatter( const linalg::Matrix& elementMatrix,
                                                    const std::vector<size_t>& locationMap )
{
    size_t size = elementMatrix.size1( );

    if (size != elementMatrix.size2( ))
        throw std::runtime_error("Can only scatter square matrix!");

    for( size_t iRow = 0; iRow < size; ++iRow )
    {
        size_t rowIndex = locationMap[iRow];

        for( size_t iColumn = 0; iColumn < size; ++iColumn )
        {
            size_t columnIndex = locationMap[iColumn];

            auto result = std::find( indices_ + indptr_[rowIndex], indices_ + indptr_[rowIndex + 1], columnIndex );

            if (!result)
                throw std::runtime_error("Entry was not found in sparsity pattern!");

            size_t index = std::distance( indices_, result );

            data_[index] += elementMatrix( iRow, iColumn );

        } // iColumn
    } // iRow
}

} // splinekernel
} // cie

#endif // CIE_SPARSE_IMPL_HPP
