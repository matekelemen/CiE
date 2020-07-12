#ifndef CSG_NTREE_INDEX_CONVERTER_HPP
#define CSG_NTREE_INDEX_CONVERTER_HPP

// --- Linalg Includes ---
#include "linalg/types.hpp"

// --- Internal Includes ---
#include "spacetreeutils.hpp"

namespace cie::csg {
namespace detail {

template <Size N, Size M>
class AbsSpaceTreeIndexConverter
{
public:
    static const Size dimension     = N;
    static const Size resolution    = M;

public:
    typedef std::array<UIntArray<N>,intPow(M,N)> index_array_type;

    AbsSpaceTreeIndexConverter();

    static constexpr Size numberOfChildren();
    static constexpr Size numberOfDataPoints();

protected:
    static constexpr index_array_type initializeIndexArray();

    static constexpr const Size       _numberOfChildren       = intPow(2,N);
    static constexpr const Size       _numberOfDataPoints     = intPow(M,N);
};

} // namespace detail


template <Size N, Size M>
class SpaceTreeIndexConverter : public detail::AbsSpaceTreeIndexConverter<N,M>
{
public:
    static constexpr const UIntArray<N>& convert(Size index);
    static constexpr Size convert(const UIntArray<N>& indexM);

    friend class SpaceTreeNode<N,M>;

protected:
    static constexpr typename detail::AbsSpaceTreeIndexConverter<N,M>::index_array_type 
        _indices = detail::AbsSpaceTreeIndexConverter<N,M>::initializeIndexArray();
};


} // namespace cie::csg

#include "../impl/indexconverter_impl.hpp"

#endif