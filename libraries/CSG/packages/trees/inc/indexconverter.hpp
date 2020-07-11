#ifndef CSG_NTREE_INDEX_CONVERTER_HPP
#define CSG_NTREE_INDEX_CONVERTER_HPP

// Linalg Includes ---
#include "linalg/types.hpp"

// --- Internal Includes ---
#include "spacetreeutils.hpp"

namespace cie::csg {

namespace detail {

template <size_t N, size_t M>
class AbsSpaceTreeIndexConverter
{
public:
    typedef std::array<UIntArray<N>,intPow(M,N)> index_array_type;

    AbsSpaceTreeIndexConverter();

    static constexpr size_t numberOfChildren();
    static constexpr size_t numberOfDataPoints();

protected:
    static constexpr index_array_type initializeIndexArray();

    static constexpr const size_t       _numberOfChildren       = intPow(2,N);
    static constexpr const size_t       _numberOfDataPoints     = intPow(M,N);
};

}


template <size_t N, size_t M>
class SpaceTreeIndexConverter : public detail::AbsSpaceTreeIndexConverter<N,M>
{
public:
    static constexpr const UIntArray<N>& convert(size_t index);
    static constexpr size_t convert(const UIntArray<N>& indexM);

    friend class SpaceTreeNode<N,M>;

protected:
    static constexpr typename detail::AbsSpaceTreeIndexConverter<N,M>::index_array_type _indices = detail::AbsSpaceTreeIndexConverter<N,M>::initializeIndexArray();
};


}

#include "../impl/indexconverter_impl.hpp"

#endif