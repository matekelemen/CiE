#ifndef CSG_NTREE_INDEX_CONVERTER_HPP
#define CSG_NTREE_INDEX_CONVERTER_HPP

#include "ntreeutils.hpp"
#include "linalgtypes.hpp"

namespace cie {
namespace csg {

template <uint8_t N, uint8_t M>
class NTreeIndexConverter
{
public:
    NTreeIndexConverter();

    const UInt8Array<N>& operator()(uint8_t index) const;
    uint8_t operator()(const UInt8Array<N>& indexM) const;

    size_t numberOfChildren() const;
    size_t numberOfDataPoints() const;

private:
    std::vector<UInt8Array<N>>  _indices;
    size_t                      _numberOfChildren;
    size_t                      _numberOfDataPoints;
};

}
}

#include "indexconverter_impl.hpp"

#endif