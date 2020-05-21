#ifndef CSG_NTREENODE_IMPL_HPP
#define CSG_NTREENODE_IMPL_HPP

// --- Internal Imports ---
#include "cmake_variables.hpp"

// --- STD Includes ---
#include <string>
#include <iostream>
#include <cmath>

namespace cie {
namespace csg {


// Initialize the static converters
template <size_t N, size_t M>
SpaceTreeIndexConverter<N,M> SpaceTreeNode<N,M>::_dataIndex = SpaceTreeIndexConverter<N,M>();

template <size_t N, size_t M>
SpaceTreeIndexConverter<N,2> SpaceTreeNode<N,M>::_centerIndex = SpaceTreeIndexConverter<N,2>();


template <size_t N, size_t M>
SpaceTreeNode<N, M>::SpaceTreeNode() :
    _data(0.0),
    _children( _dataIndex.numberOfChildren() ),
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


template <size_t N, size_t M>
SpaceTreeNode<N, M>::SpaceTreeNode(const DoubleArray<N>& center, double edgeLength) :
    _center(center),
    _data( _dataIndex.numberOfDataPoints() ,0.0),
    _children( _dataIndex.numberOfChildren() ),
    _edgeLength(edgeLength)
{
    for (auto it = _children.begin(); it!=_children.end(); ++it)
    {
        *it = nullptr;
    }
    check();
}


template <size_t N, size_t M>
SpaceTreeNode<N, M>::SpaceTreeNode(         const SpaceTreeNode<N, M>& parent, 
                                    size_t index, 
                                    const GeometryFunction<N>& geometry) :
    _data( _dataIndex.numberOfDataPoints() ),
    _children( _dataIndex.numberOfChildren() ),
    _edgeLength( parent._edgeLength/2.0 )
{
    // Check dimensions
    check();

    // Initialize children
    for (auto it = _children.begin(); it!=_children.end(); ++it)
    {
        *it = nullptr;
    }
    
    // Initialize center
    for (size_t i = 0; i < N; ++i)
        _center[i] =    parent._center[i] 
                        + parent._edgeLength/4.0 * (2.0*_centerIndex(index)[i]-1.0);

    // Copy parent data and evaluate new points
    bool evaluatePoint;

    for (size_t i = 0; i < _data.size(); ++i)
    {
        // Check if this point is shared with the parent
        evaluatePoint = false;   
        for (size_t j=0; j<N; ++j)
        {
            if ( (_dataIndex(i)[j]%2) != 0 )
                {
                    evaluatePoint = true;
                    break;
                }
        }
        // Copy parent data if shared, evaluate geometry if not
        if (!evaluatePoint)
            _data[i] = parent._data[ i/2 + (M-1)/2 * reinterpretBase<N>( _centerIndex(index), M ) ];
        else
            evaluate(geometry,i);
    }
}


template <size_t N, size_t M>
bool SpaceTreeNode<N, M>::divide(const GeometryFunction<N>& geometry, size_t level)
{
    bool result;

    #pragma omp parallel
    #pragma omp single
    result = divideRecursive( geometry, level );

    return result;
}


template <size_t N, size_t M>
bool SpaceTreeNode<N, M>::divideRecursive(const GeometryFunction<N>& geometry, size_t level)
{
    bool boundary   = false;
    bool value      = false;
    if (_data[0] > 0.0) value = true;

    // Check if boundary node
    for (const auto& data : _data)
    {
        if ( (data>0.0) != value )
        {
            boundary = true;
            break;
        }
    }

    // Divide if boundary
    if (boundary)
    {
        #pragma omp task shared(_children, geometry, level)
        for (auto it = _children.begin(); it != _children.end(); ++it)
        {
            *it = std::make_unique<SpaceTreeNode>(  *this,
                                                    std::distance(_children.begin(),it),
                                                    geometry);
            if (level>1) (*it)->divideRecursive(geometry, level-1);
        }
    }

    // Reset children if not boundary
    else
        wipe();

    return boundary;
}


// Write edge length, center coordinates and data to the output stream, then
// call write on the children
template <size_t N, size_t M>
void SpaceTreeNode<N,M>::write(std::ostream& file) const
{
    writeSpaceTree<N,M>(*this,file);
    for (auto& child : _children)
        if (child != nullptr) 
            child->write(file);
}


// Call wipe on children then delete them
template <size_t N, size_t M>
void SpaceTreeNode<N,M>::wipe()
{
    #pragma omp task shared(_children)
    for (auto& child : _children)
    {
        if (child != nullptr)
        {
            child->wipe();
            child.reset();
        }
    }
}


// Evaluate the geometry at a specific point defined by its index
template <size_t N, size_t M>
void SpaceTreeNode<N, M>::evaluate(const GeometryFunction<N>& geometry, size_t index)
{   
    _data[index] = geometry(pointCoordinates(index));
}


// Evaluate the geometry at all the points
template <size_t N, size_t M>
void SpaceTreeNode<N, M>::evaluate(const GeometryFunction<N>& geometry)
{
    for (size_t i=0; i<_data.size(); ++i)
        evaluate(geometry,i);
}


template <size_t N, size_t M>
DoubleArray<N> SpaceTreeNode<N, M>::pointCoordinates(const UIntArray<N>& indexN) const
{
    DoubleArray<N> point;
    double dx = _edgeLength / ((double)M-1.0);
    for (size_t j = 0; j < N; ++j)
        point[j] = _center[j] + dx*(indexN[j] - (M-1.0)/2.0);

    return point;
}


template <size_t N, size_t M>
DoubleArray<N> SpaceTreeNode<N, M>::pointCoordinates(size_t index) const
{
    return pointCoordinates( _dataIndex(index) );
}


template <size_t N, size_t M>
void SpaceTreeNode<N, M>::check() const
{
    if (M%2 != 1)
        throw std::runtime_error("Number of partitions must be odd and >1! (" + std::to_string(M) + ")");
}


template <size_t N, size_t M>
DoubleArray<N>& SpaceTreeNode<N, M>::center()
{
    return _center;
}


template <size_t N, size_t M>
const DoubleArray<N>& SpaceTreeNode<N, M>::center() const
{
    return _center;
}


template <size_t N, size_t M>
const DoubleVector& SpaceTreeNode<N, M>::data() const
{
    return _data;
}


template <size_t N, size_t M>
const std::vector<SpaceTreeNodePtr<N, M>>& SpaceTreeNode<N, M>::children() const
{
    return _children;
}


template <size_t N, size_t M>
const SpaceTreeNode<N, M>& SpaceTreeNode<N, M>::child(size_t index) const
{
    if (_children[index]!=nullptr)
        return *(_children[index]);
    else
        throw std::runtime_error("Unable to dereference child node (nullptr)!");
}


template <size_t N, size_t M>
double SpaceTreeNode<N, M>::edgeLength() const
{
    return _edgeLength;
}


}
}

#endif