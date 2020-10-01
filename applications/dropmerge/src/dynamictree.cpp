/*
#include "../inc/dynamictree.hpp"

namespace cie {
namespace csg {


DynamicTree::DynamicTree(   const DoubleArray<N>& center, 
                            double edgeLength ) :
    SpaceTreeNode<N,M>(center, edgeLength),
    _drop(exponentialMergeFunction<N>),
    _offset(0.0)
{
    _geometry = [this](const DoubleArray<N>& point)
                    {return this->_drop(point,_offset);};
    SpaceTreeNode<N,M>::evaluate(_geometry);
}


void DynamicTree::divide(size_t level)
{
    SpaceTreeNode<N,M>::divide(_geometry, level);
}


void DynamicTree::offset(double value)
{
    SpaceTreeNode<N,M>::wipe();
    _offset = value;
    SpaceTreeNode<N,M>::evaluate(_geometry);
}


}
}
*/