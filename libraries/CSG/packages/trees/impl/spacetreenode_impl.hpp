#ifndef CIE_CSG_SPACETREENODE_IMPL_HPP
#define CIE_CSG_SPACETREENODE_IMPL_HPP

// --- Internal Includes ---
#include "cmake_variables.hpp"

// --- STL Includes ---
#include <string>
#include <iostream>
#include <cmath>

namespace cie::csg {


// Initialize static index converters
template <size_t N, size_t M>
SpaceTreeIndexConverter<N,M> SpaceTreeNode<N,M>::_dataIndex = SpaceTreeIndexConverter<N,M>();

template <size_t N, size_t M>
SpaceTreeIndexConverter<N,2> SpaceTreeNode<N,M>::_centerIndex = SpaceTreeIndexConverter<N,2>();

/*
template <class CT,class ST, class SP, concepts::STLContainer VCT>
template <class ...Args>
SpaceTreeNode_<CT,ST,SP,VCT>::SpaceTreeNode_( Args&&... args ) :
    SpaceTreeNode_<CT,ST,SP,VCT>::cell_type( std::forward<Args>(args)... )
{
}
*/


template <size_t N, size_t M>
SpaceTreeNode<N, M>::SpaceTreeNode() :
    _data(),
    _children( ),
    _edgeLength(2.0)
{
    static_assert( M<2 || M%2 != 1, "Number of partitions must be odd and greater than 1!" );

    for (auto& child : _children)
        child.reset();
    for (auto& coordinate : _center)
        coordinate = 0.0;
}


template <size_t N, size_t M>
SpaceTreeNode<N, M>::SpaceTreeNode( const typename SpaceTreeNode<N,M>::coordinate_container_type& center, 
                                    double edgeLength) :
    _center(center),
    _data(),
    _children(),
    _edgeLength(edgeLength)
{
    static_assert( M>1 || M%2 != 1, "Number of partitions must be odd and greater than 1!" );

    for (auto& child : _children)
        child.reset();
}


template <size_t N, size_t M>
SpaceTreeNode<N, M>::SpaceTreeNode( SpaceTreeNode<N, M>& parent, 
                                    size_t index, 
                                    const GeometryFunction<N>& geometry,
                                    bool autoEvaluate ) :
    _data(),
    _children(),
    _edgeLength( parent._edgeLength/2.0 )
{
    // Initialize center
    for (size_t i = 0; i < N; ++i)
        _center[i] =    parent._center[i] 
                        + parent._edgeLength/4.0 * (2.0*_centerIndex.convert(index)[i]-1.0);

    // Copy parent data and evaluate new points
    bool evaluatePoint;

    for (size_t i = 0; i < _data.size(); ++i)
    {
        // Check if this point is shared with the parent
        evaluatePoint = false;   
        for (size_t j=0; j<N; ++j)
        {
            if ( (_dataIndex.convert(i)[j]%2) != 0 )
                {
                    evaluatePoint = true;
                    break;
                }
        }
        // Copy parent data if shared, evaluate geometry if not
        if (!evaluatePoint)
            _data[i] = parent._data[ i/2 + (M-1)/2 * reinterpretBase<N>( _centerIndex.convert(index), M ) ];
        else
            if (autoEvaluate)
                evaluate( geometry, i );
            else
                requestEvaluation( parent, pointCoordinates(i), &_data[i] );
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

    if (_data[0] > 0.0) 
        value = true;

    // Check if boundary node
    for (const auto& data : _data)
        if ( (data>0.0) != value )
        {
            boundary = true;
            break;
        }

    // Divide if boundary
    if (boundary)
    {
        #ifndef MSVC
        #pragma omp task
        #endif
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
    #ifndef MSVC
    #pragma omp task shared(_children)
    #endif
    for (auto& child : _children)
        if (child != nullptr)
        {
            child->wipe();
            child.reset();
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
typename SpaceTreeNode<N,M>::coordinate_container_type SpaceTreeNode<N, M>::pointCoordinates(const UIntArray<N>& indexN) const
{
    typename SpaceTreeNode<N,M>::coordinate_container_type point;
        
    double dx       = _edgeLength / ((double)M-1.0);
    double offset   = (M-1.0) / 2.0;
    auto centerIt   = _center.begin();
    auto indexIt    = indexN.begin();

    for (   auto pointIt=point.begin(); 
            pointIt!=point.end(); 
            ++pointIt,++centerIt,++indexIt  )
                *pointIt = *centerIt + dx * (*indexIt - offset);

    return point;
}


template <size_t N, size_t M>
typename SpaceTreeNode<N,M>::coordinate_container_type SpaceTreeNode<N, M>::pointCoordinates(size_t index) const
{
    return pointCoordinates( _dataIndex.convert(index) );
}


template <size_t N, size_t M>
typename SpaceTreeNode<N,M>::coordinate_container_type& SpaceTreeNode<N, M>::center()
{
    return _center;
}


template <size_t N, size_t M>
const typename SpaceTreeNode<N,M>::coordinate_container_type& SpaceTreeNode<N, M>::center() const
{
    return _center;
}


template <size_t N, size_t M>
const typename SpaceTreeNode<N,M>::data_container_type& SpaceTreeNode<N, M>::data() const
{
    return _data;
}


template <size_t N, size_t M>
const typename SpaceTreeNode<N,M>::child_container_type& SpaceTreeNode<N, M>::children() const
{
    return _children;
}


template <size_t N, size_t M>
const SpaceTreeNode<N, M>& SpaceTreeNode<N, M>::child(size_t index) const
{
    if (_children[index]!=nullptr)
        return *(_children[index]);
    else
        CIE_THROW( NullPtrException, "Unable to dereference child node" )
}


template <size_t N, size_t M>
double SpaceTreeNode<N, M>::edgeLength() const
{
    return _edgeLength;
}


template<size_t N, size_t M>
bool SpaceTreeNode<N,M>::divideOffload( const GeometryFunction<N>& geometry, size_t level )
{
    bool result;

    //#pragma omp parallel shared(_children,_data,_evaluationRequests)
    //#pragma omp single
    result = divideOffloadRecursive( geometry, level );

    return result;
}


template<size_t N, size_t M>
bool SpaceTreeNode<N,M>::divideOffloadRecursive( const GeometryFunction<N>& geometry, size_t level )
{
    bool boundary   = false;
    bool value      = false;

    if (_data[0] > 0.0) 
        value = true;

    // Check if boundary node
    for (const auto& data : _data)
        if ( (data>0.0) != value )
        {
            boundary = true;
            break;
        }

    // Divide if boundary
    if (boundary)
    {
        //#pragma omp parallel for shared(_evaluationRequests, _children, _data, geometry, level)
        #ifndef MSVC
        #pragma omp parallel for simd
        #endif
        for (auto it = _children.begin(); it != _children.end(); ++it)
            *it = std::make_unique<SpaceTreeNode>(  *this,
                                                    std::distance(_children.begin(),it),
                                                    geometry,
                                                    false);

        // Offload evaluation
        //std::vector<double> values(_evaluationRequests.size());
        //#pragma omp parallel for shared(_evaluationRequests,_children, _data, geometry, level)
        //#pragma omp target teams distribute parallel for map(from:_evaluationRequests) map(to:values)
        #ifndef MSVC
        #pragma omp parallel for simd
        #endif
        for (size_t i=0; i<_evaluationRequests.size(); ++i)
        {
            //values[i] = geometry(_evaluationRequests[i].first);
            *_evaluationRequests[i].second = geometry(_evaluationRequests[i].first);
        }
        
        //for (size_t i=0; i<_evaluationRequests.size(); ++i)
        //    *_evaluationRequests[i].second = values[i];

        _evaluationRequests.clear();

        // Divide children if necessary
        if (level>1)
            for (auto& child : _children)
                child->divideOffloadRecursive(geometry, level-1);
    }

    // Reset children if not boundary
    else
        wipe();

    return boundary;
}


template<size_t N, size_t M>
void SpaceTreeNode<N,M>::requestEvaluation( SpaceTreeNode<N,M>& parent,
                                            const typename SpaceTreeNode<N,M>::coordinate_container_type& point, 
                                            double* dataPtr )
{
    #pragma omp critical(requestUpdate)
    parent._evaluationRequests.emplace_back( point, dataPtr );
}


} // namespace cie::csg

#endif