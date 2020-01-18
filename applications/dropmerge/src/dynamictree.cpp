#include "../inc/dynamictree.hpp"

namespace cie {
namespace csg {


DynamicTree::DynamicTree(   const DoubleArray<N>& center, 
                            double edgeLength ) :
    NTreeNode<N,M>(center, edgeLength),
    _drop(exponentialMergeFunction<N>),
    _offset(0.0)
{
    _geometry = [this](const DoubleArray<N>& point)
                    {return this->_drop(point,_offset);};
    NTreeNode<N,M>::evaluate(_geometry);
}


void DynamicTree::divide(size_t level)
{
    NTreeNode<N,M>::divide(_geometry, level);
}


void DynamicTree::offset(double value)
{
    NTreeNode<N,M>::wipe();
    _offset = value;
    NTreeNode<N,M>::evaluate(_geometry);
}


}
}