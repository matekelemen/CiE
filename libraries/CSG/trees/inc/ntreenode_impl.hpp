#ifndef CSG_NTREENODE_IMPL_HPP
#define CSG_NTREENODE_IMPL_HPP
#include <string>
#include <iostream>

namespace cie {
namespace csg {


template <uint8_t N, uint8_t M>
NTreeNode<N, M>::NTreeNode() :
    _children(intPow(2,N)),
    _edgeLength(2.0)
{
    for (auto it = _children.begin(); it!=_children.end(); ++it)
    {
        *it = nullptr;
    }
    for (auto it = _center.begin(); it != _center.end(); ++it)
    {
        *it = 0.0;
    }
    check();
}


template <uint8_t N, uint8_t M>
NTreeNode<N, M>::NTreeNode(const DoubleArray<N>& center, double edgeLength) :
    _center(center),
    _data(intPow(M, N),0.0),
    _children(intPow(2, N)),
    _edgeLength(edgeLength)
{
    for (auto it = _children.begin(); it!=_children.end(); ++it)
    {
        *it = nullptr;
    }
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

    // Initialize children
    for (auto it = _children.begin(); it!=_children.end(); ++it)
    {
        *it = nullptr;
    }
    
    // Initialize center
    UInt8Array<N> indexN = baseN<N>(index, 2);
    for (uint8_t i = 0; i < N; ++i)
    {
        _center[i] = parent._center[i] + parent._edgeLength/4.0 * (2.0*indexN[i]-1.0);
    }

    // Copy parent data and evaluate new points
    bool evaluatePoint;

    for (size_t i = 0; i < _data.size(); ++i)
    {
        // Convert index
        baseN<N>(i,M,indexN);
        // Check if this point is shared with the parent
        evaluatePoint = true;   
        for (size_t j = 0; j < indexN.size(); ++j)
        {
            if (indexN[j] % 2 != 0)
            {
                evaluatePoint = true;
                break;
            }
        }
        // Copy parent data if shared, evaluate geometry if not
        if (!evaluatePoint)
        {
            for (auto it=indexN.begin();it!=indexN.end();++it) 
                *it /= 2.0;
            _data[i] = parent._data[ base10<N>(indexN,M) ];
        }
        else
        {
            evaluate(geometry,i);
        }
    }
}


template <uint8_t N, uint8_t M>
bool NTreeNode<N, M>::divide(const GeometryFunction<N>& geometry, uint8_t level)
{
    bool boundary = false;
    bool value = false;
    if (_data[0] > 0.0) value = true;

    // Check if boundary node
    for (auto it = _data.begin(); it != _data.end(); ++it)
    {
        if ( (*it>0.0) != value )
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


// Write edge length, center coordinates and data to the output stream, then
// call write on the children
template <uint8_t N, uint8_t M>
void NTreeNode<N,M>::write(std::ostream& file) const
{
    writeNTree<N,M>(*this,file);
    for (auto it=_children.begin(); it!=_children.end(); ++it)
        if (*it!=nullptr) 
            (**it).write(file);
}


// Call wipe on children then delete them
template <uint8_t N, uint8_t M>
void NTreeNode<N,M>::wipe()
{
    for (auto it=_children.begin(); it!=_children.end(); ++it)
    {
        if (*it != nullptr)
        {
            (*it)->wipe();
            it->reset();
        }
    }
}


// Evaluate the geometry at a specific point defined by its index
template <uint8_t N, uint8_t M>
void NTreeNode<N, M>::evaluate(const GeometryFunction<N>& geometry, size_t index)
{   
    _data[index] = geometry(pointCoordinates(index));
}


// Evaluate the geometry at all the points
template <uint8_t N, uint8_t M>
void NTreeNode<N, M>::evaluate(const GeometryFunction<N>& geometry)
{
    for (size_t i=0; i<_data.size(); ++i)
        evaluate(geometry,i);
}


template <uint8_t N, uint8_t M>
DoubleArray<N> NTreeNode<N, M>::pointCoordinates(const UInt8Array<N>& indexN) const
{
    DoubleArray<N> point;
    double dx = _edgeLength / ((double)M-1.0);
    for (uint8_t j = 0; j < N; ++j)
        point[j] = _center[j] + dx*(indexN[j] - (M-1.0)/2.0);

    return point;
}


template <uint8_t N, uint8_t M>
DoubleArray<N> NTreeNode<N, M>::pointCoordinates(size_t index) const
{
    UInt8Array<N> indexN = baseN<N>(index,M);
    return pointCoordinates( indexN );
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
const DoubleVector& NTreeNode<N, M>::data() const
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
    if (_children[index]!=nullptr)
        return *(_children[index]);
    else
        throw std::runtime_error("Unable to dereference child node (nullptr)!");
}


template <uint8_t N, uint8_t M>
double NTreeNode<N, M>::edgeLength() const
{
    return _edgeLength;
}


}
}

#endif