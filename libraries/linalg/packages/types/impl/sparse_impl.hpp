#ifndef LINALG_SPARSE_IMPL_HPP
#define LINALG_SPARSE_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- STL Includes ---
#include <algorithm>
#include <numeric>

namespace cie::linalg {


template<concepts::NumericType IndexType, concepts::NumericType ValueType>
CompressedSparseRowMatrix<IndexType,ValueType>::CompressedSparseRowMatrix( ) :
    indices_( nullptr ), indptr_( nullptr ), data_( nullptr ),
    size1_( 0 ), size2_( 0 )
{

}

template<concepts::NumericType IndexType, concepts::NumericType ValueType>
std::tuple<IndexType*, IndexType*, ValueType*, Size> CompressedSparseRowMatrix<IndexType,ValueType>::release( )
{
    std::tuple<IndexType*, IndexType*, ValueType*, Size> sparseDataStructure { indices_, indptr_, data_, size1_ };

    indices_    = nullptr;
    indptr_     = nullptr;
    data_       = nullptr;

    size1_ = 0;
    size2_ = 0;

    return sparseDataStructure;
}

template<concepts::NumericType IndexType, concepts::NumericType ValueType>
IndexType CompressedSparseRowMatrix<IndexType,ValueType>::nnz( ) const
{
    return indptr_[size1_];
}

template<concepts::NumericType IndexType, concepts::NumericType ValueType>
Size CompressedSparseRowMatrix<IndexType,ValueType>::size1( ) const
{
    return size1_;
}

template<concepts::NumericType IndexType, concepts::NumericType ValueType>
Size CompressedSparseRowMatrix<IndexType,ValueType>::size2( ) const
{
    return size2_;
}

template<concepts::NumericType IndexType, concepts::NumericType ValueType>
void CompressedSparseRowMatrix<IndexType,ValueType>::cleanup( )
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

template<concepts::NumericType IndexType, concepts::NumericType ValueType>
ValueType CompressedSparseRowMatrix<IndexType,ValueType>::operator ()( Size i,
                                               Size j ) const
{
    auto result = std::find( indices_ + indptr_[i], indices_ + indptr_[i + 1], j );

    if( result )
    {
        return *result;
    }

    return 0.0;
}

template<concepts::NumericType IndexType, concepts::NumericType ValueType>
std::vector<ValueType> CompressedSparseRowMatrix<IndexType,ValueType>::operator*( const std::vector<ValueType>& vector )
{
    if (vector.size() != size2_)
        CIE_THROW( OutOfRangeException, "Inconsistent sizes in matrix vector multiplication" )

    std::vector<ValueType> result( size1_, 0.0 );

    for( Size i = 0; i < size1_; ++i )
    {
        for( IndexType j = indptr_[i]; j < indptr_[i + 1]; ++j )
        {
            result[i] += data_[j] * vector[indices_[j]];
        }
    }

    return result;
}

template<concepts::NumericType IndexType, concepts::NumericType ValueType>
CompressedSparseRowMatrix<IndexType,ValueType>::~CompressedSparseRowMatrix( )
{
    cleanup( );
}



template<concepts::NumericType IndexType, concepts::NumericType ValueType>
void CompressedSparseRowMatrix<IndexType,ValueType>::allocate( const std::vector<std::vector<Size>>& locationMaps )
{
    cleanup( );

    Size numberOfElements = locationMaps.size( );
    Size globalNumberOfDofs = 0;

    for( const auto& locationMap : locationMaps )
    {
        Size maxElementIndex = *std::max_element( locationMap.begin( ), locationMap.end( ) ) + 1;

        if( maxElementIndex > globalNumberOfDofs )
        {
            globalNumberOfDofs = maxElementIndex;
        }
    }

    std::vector<std::vector<Size>> dofToElementCoupling( globalNumberOfDofs );

    for( Size iElement = 0; iElement < numberOfElements; ++iElement )
    {
        for( Size dofIndex : locationMaps[iElement] )
        {
            dofToElementCoupling[dofIndex].push_back( iElement );
        }
    }

    std::vector<std::vector<Size>> dofToDofCoupling( globalNumberOfDofs );
    std::vector<Size> tmp;

    for( Size iDof = 0; iDof < globalNumberOfDofs; ++iDof )
    {
        if (dofToElementCoupling[iDof].size( ) <= 0)
            CIE_THROW( std::runtime_error, "DoF without connected element: " + std::to_string(iDof) )

        tmp.resize( 0 );

        for( Size iElement = 0; iElement < dofToElementCoupling[iDof].size( ); ++iElement )
        {
            const auto& locationMap = locationMaps[dofToElementCoupling[iDof][iElement]];

            tmp.insert( tmp.end( ), locationMap.begin( ), locationMap.end( ) );
        }

        std::sort( tmp.begin( ), tmp.end( ) );
        auto end = std::unique( tmp.begin( ), tmp.end( ) );

        dofToDofCoupling[iDof].insert( dofToDofCoupling[iDof].end( ), tmp.begin( ), end );
    }

    std::vector<std::vector<Size>>{ }.swap( dofToElementCoupling );

    size1_ = globalNumberOfDofs;
    size2_ = globalNumberOfDofs;

    indptr_ = new IndexType[size1_ + 1];
    indptr_[0] = 0;

    auto vectorSizePredicate = []( const std::vector<Size>& dofs ) { return static_cast<IndexType>( dofs.size( ) ); };

    // Fill indptr with cumulative sum of the vector sizes of dofToDofCoupling
    std::transform( dofToDofCoupling.begin( ), dofToDofCoupling.end( ), indptr_ + 1, vectorSizePredicate );
    std::partial_sum( indptr_, indptr_ + size1_ + 1, indptr_ );

    IndexType nnz = indptr_[size1_];

    indices_ = new IndexType[nnz];

    // Concatenate dof indices
    IndexType* currentRow = indices_;

    for( const std::vector<Size>& dofs : dofToDofCoupling )
    {
        std::copy( dofs.begin( ), dofs.end( ), currentRow );

        currentRow += dofs.size( );
    }

    std::vector<std::vector<Size>>{ }.swap( dofToDofCoupling );

    // Resize data
    data_ = new ValueType[nnz];

    std::fill( data_, data_ + nnz, 0.0 );
}

template<concepts::NumericType IndexType, concepts::NumericType ValueType>
void CompressedSparseRowMatrix<IndexType,ValueType>::scatter( const linalg::Matrix<ValueType>& elementMatrix,
                                                    const std::vector<Size>& locationMap )
{
    Size size = elementMatrix.size1( );

    if (size != elementMatrix.size2( ))
        CIE_THROW( std::runtime_error, "Can only scatter square matrix!" )

    for( Size iRow = 0; iRow < size; ++iRow )
    {
        Size rowIndex = locationMap[iRow];

        for( Size iColumn = 0; iColumn < size; ++iColumn )
        {
            Size columnIndex = locationMap[iColumn];

            auto result = std::find( indices_ + indptr_[rowIndex], indices_ + indptr_[rowIndex + 1], columnIndex );

            if (!result)
                CIE_THROW( std::runtime_error, "Entry was not found in sparsity pattern!" )

            Size index = std::distance( indices_, result );

            data_[index] += elementMatrix( iRow, iColumn );

        } // iColumn
    } // iRow
}

} // cie::splinekernel

#endif // CIE_SPARSE_IMPL_HPP
