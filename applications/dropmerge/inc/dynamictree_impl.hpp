#ifndef DROPMERGE_DYNAMIC_TREE_IMPL_HPP
#define DROPMERGE_DYNAMIC_TREE_IMPL_HPP

namespace cie {
namespace csg {


DynamicTree::DynamicTree(  const DoubleArray<N>& center, 
                                double edgeLength ) :
    NTreeNode<N,M>(center, edgeLength),
    _geometry(exponentialMergeFunction<N>)
{
    this->evaluate(_geometry);
}


DynamicTree::DynamicTree( const DynamicTree& parent, uint8_t index ) :
    NTreeNode<N,M>(parent, index, parent._geometry),
    _geometry(parent._geometry)
{
}


void DynamicTree::divide(uint8_t level)
{
    this->template divide<N,M>(_geometry, level);
}


}
}

#endif