#ifndef CSG_NTREE_INDEX_CONVERTER_IMPL_HPP
#define CSG_NTREE_INDEX_CONVERTER_IMPL_HPP

#include <iterator>

namespace cie {
namespace csg {


template <uint8_t N, uint8_t M>
NTreeIndexConverter<N,M>::NTreeIndexConverter() :
    _numberOfChildren(intPow(2,N)),
    _numberOfDataPoints(intPow(M,N))
{
    _indices = std::vector<UInt8Array<N>>( _numberOfDataPoints );

    for (size_t i=0; i<_numberOfDataPoints; ++i)
        _indices[i] = baseN<N>(i,M);
}


template <uint8_t N, uint8_t M>
const UInt8Array<N>& NTreeIndexConverter<N,M>::operator()(uint8_t index) const
{
    return _indices[index];
}


template <uint8_t N, uint8_t M>
uint8_t NTreeIndexConverter<N,M>::operator()(const UInt8Array<N>& indexN) const
{
    return std::distance( _indices.begin(), &indexN );
}


template <uint8_t N, uint8_t M>
size_t NTreeIndexConverter<N,M>::numberOfChildren() const
{
    return _numberOfChildren;
}


template <uint8_t N, uint8_t M>
size_t NTreeIndexConverter<N,M>::numberOfDataPoints() const
{
    return _numberOfDataPoints;
}


}
}

#endif