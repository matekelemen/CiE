#ifndef CSG_NTREENODE_IMPL_HPP
#define CSG_NTREENODE_IMPL_HPP
#include <string>

namespace cie {
namespace csg {


template <uint8_t N, uint8_t M>
NTreeNode<N, M>::NTreeNode() :
    _children(intPow(2,N)),
    _edgeLength(2.0)
{
    for (auto it = _center.begin(); it != _center.end(); ++it)
    {
        *it = 0.0;
    }
    check();
}


template <uint8_t N, uint8_t M>
NTreeNode<N, M>::NTreeNode(const DoubleArray<N>& center, double edgeLength) :
    _center(center),
    _data(intPow(M, N)),
    _children(intPow(2, N)),
    _edgeLength(edgeLength)
{
    check();
}


template <uint8_t N, uint8_t M>
NTreeNode<N, M>::NTreeNode(         const NTreeNode<N, M>& parent, 
                                    uint8_t index, 
                                    const GeometryFunction<N>& geometry) :
    _data(intPow(M, N)),
    _children(intPow(2, N)),
    _edgeLength(parent._edgeLength/2.0)
{
    // Check dimensions
    check();
    
    // Initialize center
    UInt8Array<N> indexN = baseN<N>(index, 2);
    for (uint8_t i = 0; i < N; ++i)
    {
        _center[i] = parent._center[i] + parent._edgeLength/4.0 * (2.0*indexN[i]-1.0);
    }

    // Copy parent data and evaluate new points
    bool evaluate;
    DoubleArray<N> point;
    double dx = _edgeLength / (M-1.0);

    for (size_t i = 0; i < _data.size(); ++i)
    {
        // Convert index
        baseN<N>(i,M,indexN);
        // Check if this point is shared with the parent
        evaluate = true;   
        for (uint8_t j = 0; j < indexN.size(); ++j)
        {
            if (indexN[j] % 2 != 0)
            {
                evaluate = false;
                break;
            }
        }
        // Copy parent data if shared, evaluate geometry if not
        if (!evaluate)
        {
            for (auto it=indexN.begin();it!=indexN.end();++it) *it /= 2;
            _data[i] = parent._data[ base10<N>(indexN,M) ];
        }
        else
        {
            for (uint8_t j = 0; j < N; ++j)
            {
                point[j] = _center[j] + dx*(j - (M-1.0)/2.0);
            }
            _data[i] = geometry(point);
        }
    }
}


template <uint8_t N, uint8_t M>
bool NTreeNode<N, M>::divide(const GeometryFunction<N>& geometry, uint8_t level)
{
    bool boundary = false;
    uint8_t value = _data[0];

    // Check if boundary node
    for (auto it = _data.begin(); it != _data.end(); ++it)
    {
        if (*it != value)
        {
            boundary = true;
            break;
        }
    }

    // Divide if boundary
    if (boundary)
    {
        for (auto it = _children.begin(); it != _children.end(); ++it)
        {
            *it = std::make_unique<NTreeNode>(  *this,
                                                std::distance(_children.begin(),it),
                                                geometry);
            if (level>1) (*it)->divide(geometry, level-1);
        }
    }

    return boundary;
}


template <uint8_t N, uint8_t M>
void NTreeNode<N, M>::check() const
{
    if (M%2 != 1)
        throw std::runtime_error("Number of partitions must be odd and >1! (" + std::to_string(M) + ")");
}


template <uint8_t N, uint8_t M>
DoubleArray<N>& NTreeNode<N, M>::center()
{
    return _center;
}


template <uint8_t N, uint8_t M>
const DoubleArray<N>& NTreeNode<N, M>::center() const
{
    return _center;
}


template <uint8_t N, uint8_t M>
const UInt8Vector& NTreeNode<N, M>::data() const
{
    return _data;
}


template <uint8_t N, uint8_t M>
const std::vector<NTreeNodePtr<N, M>>& NTreeNode<N, M>::children() const
{
    return _children;
}


template <uint8_t N, uint8_t M>
const NTreeNode<N, M>& NTreeNode<N, M>::child(size_t index) const
{
    return *(_children[index]);
}


}
}

#endif