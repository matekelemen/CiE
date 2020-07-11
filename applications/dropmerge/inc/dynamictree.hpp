#ifndef DROPMERGE_DYNAMIC_TREE_HPP
#define DROPMERGE_DYNAMIC_TREE_HPP

// --- CSG Includes ---
#include <csg/trees.hpp>

// --- Internal Includes ---
#include "mergefunction.hpp"


// Set dimension and subdivision (needed for python bindings)
const size_t N = 2;
const size_t M = 7;

using DropFunction = std::function<double(const std::array<double,N>&, double)>;

namespace cie {
namespace csg {


class DynamicTree : public SpaceTreeNode<N,M>
{
public:
    DynamicTree( const DoubleArray<N>& center, double edgeLength );

    void divide(size_t level);
    void offset(double value);

    DropFunction        _drop;
    GeometryFunction<N> _geometry;
    double              _offset;
};


}
}

#endif