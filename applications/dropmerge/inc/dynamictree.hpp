#ifndef DROPMERGE_DYNAMIC_TREE_HPP
#define DROPMERGE_DYNAMIC_TREE_HPP

#include "../../libraries/CSG/trees/inc/ntreeutils.hpp"
#include "../../libraries/CSG/trees/inc/ntreenode.hpp"
#include "mergefunction.hpp"

const uint8_t N = 2;
const uint8_t M = 3;
using DropFunction = std::function<double(const std::array<double,N>&, double)>;

namespace cie {
namespace csg {


class DynamicTree : public NTreeNode<N,M>
{
public:
    DynamicTree( const DoubleArray<N>& center, double edgeLength );
    DynamicTree( const DynamicTree& parent, uint8_t index );

    void divide(uint8_t level);
    void offset(double value);

    DropFunction        _drop;
    GeometryFunction<N> _geometry;
    double              _offset;
};


}
}

#endif