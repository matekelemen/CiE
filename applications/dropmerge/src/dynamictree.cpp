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
    this->evaluate(_geometry);
}


DynamicTree::DynamicTree( const DynamicTree& parent, uint8_t index ) :
    NTreeNode<N,M>(parent, index, parent._geometry),
    _geometry(parent._geometry),
    _offset(parent._offset)
{
}


void DynamicTree::divide(uint8_t level)
{
    NTreeNode<N,M>::divide(_geometry, level);
}


void DynamicTree::offset(double value)
{
    NTreeNode<N,M>::wipe();
    _offset = value;
}


}
}