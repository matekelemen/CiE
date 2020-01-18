#ifndef CSG_NTREE_INDEX_CONVERTER_HPP
#define CSG_NTREE_INDEX_CONVERTER_HPP

#include "spacetreeutils.hpp"
#include "linalgtypes.hpp"

namespace cie {
namespace csg {

template <size_t N, size_t M>
class SpaceTreeIndexConverter
{
public:
    SpaceTreeIndexConverter();

    const UIntArray<N>& operator()(size_t index) const;
    size_t operator()(const UIntArray<N>& indexM) const;

    size_t numberOfChildren() const;
    size_t numberOfDataPoints() const;

private:
    std::vector<UIntArray<N>>  _indices;
    size_t                      _numberOfChildren;
    size_t                      _numberOfDataPoints;
};

}
}

#include "indexconverter_impl.hpp"

#endif