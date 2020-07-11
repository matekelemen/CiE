#ifndef CSG_NTREE_INDEX_CONVERTER_IMPL_HPP
#define CSG_NTREE_INDEX_CONVERTER_IMPL_HPP

#include <iterator>

namespace cie::csg {

namespace detail {

template <size_t N, size_t M>
constexpr typename AbsSpaceTreeIndexConverter<N,M>::index_array_type
AbsSpaceTreeIndexConverter<N,M>::initializeIndexArray()
{
    typename AbsSpaceTreeIndexConverter<N,M>::index_array_type indexArray;
    for (size_t i=0; i<_numberOfDataPoints; ++i)
        indexArray[i] = baseN<N>(i,M);
    return indexArray;
}


template <size_t N, size_t M>
AbsSpaceTreeIndexConverter<N,M>::AbsSpaceTreeIndexConverter()
{
    //_indices = std::vector<UIntArray<N>>( _numberOfDataPoints );
    //for (size_t i=0; i<_numberOfDataPoints; ++i)
    //    _indices[i] = baseN<N>(i,M);
}


template <size_t N, size_t M>
constexpr size_t AbsSpaceTreeIndexConverter<N,M>::numberOfChildren()
{
    return _numberOfChildren;
}


template <size_t N, size_t M>
constexpr size_t AbsSpaceTreeIndexConverter<N,M>::numberOfDataPoints()
{
    return _numberOfDataPoints;
}

}


template <size_t N, size_t M>
constexpr const UIntArray<N>& SpaceTreeIndexConverter<N,M>::convert(size_t index)
{
    return _indices[index];
}


template <size_t N, size_t M>
constexpr size_t SpaceTreeIndexConverter<N,M>::convert(const UIntArray<N>& indexN)
{
    return std::distance( _indices.begin(), &indexN );
}


}

#endif