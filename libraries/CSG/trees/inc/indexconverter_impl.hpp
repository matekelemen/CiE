#ifndef CSG_NTREE_INDEX_CONVERTER_IMPL_HPP
#define CSG_NTREE_INDEX_CONVERTER_IMPL_HPP

#include <iterator>

namespace cie {
namespace csg {


template <size_t N, size_t M>
constexpr const size_t SpaceTreeIndexConverter<N,M>::_numberOfChildren = intPow(2,N);


template <size_t N, size_t M>
constexpr const size_t SpaceTreeIndexConverter<N,M>::_numberOfDataPoints = intPow(M,N);


template <size_t N, size_t M>
SpaceTreeIndexConverter<N,M>::SpaceTreeIndexConverter()
{
    _indices = std::vector<UIntArray<N>>( _numberOfDataPoints );

    for (size_t i=0; i<_numberOfDataPoints; ++i)
        _indices[i] = baseN<N>(i,M);
}


template <size_t N, size_t M>
const UIntArray<N>& SpaceTreeIndexConverter<N,M>::operator()(size_t index) const
{
    return _indices[index];
}


template <size_t N, size_t M>
size_t SpaceTreeIndexConverter<N,M>::operator()(const UIntArray<N>& indexN) const
{
    return std::distance( _indices.begin(), &indexN );
}


template <size_t N, size_t M>
size_t SpaceTreeIndexConverter<N,M>::numberOfChildren() const
{
    return _numberOfChildren;
}


template <size_t N, size_t M>
size_t SpaceTreeIndexConverter<N,M>::numberOfDataPoints() const
{
    return _numberOfDataPoints;
}


}
}

#endif