#ifndef DROPMERGE_DYNAMIC_TREE_HPP
#define DROPMERGE_DYNAMIC_TREE_HPP

#include "../../libraries/CSG/trees/inc/ntreeutils.hpp"
#include "mergefunction.hpp"

const uint8_t N = 3;
const uint8_t M = 5;
using dropFunction = std::function<double(const std::array<double,N>&)>;

namespace cie {
namespace csg {


class DynamicTree : public NTreeNode<N,M>
{
public:
    DynamicTree( const DoubleArray<N>& center, double edgeLength );
    DynamicTree( const DynamicTree& parent, uint8_t index );

    void divide(uint8_t level);

    dropFunction _geometry;
};


}
}

#include "dynamictree_impl.hpp"

#endif