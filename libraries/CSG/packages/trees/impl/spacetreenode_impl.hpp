#ifndef CIE_CSG_SPACETREENODE_IMPL_HPP
#define CIE_CSG_SPACETREENODE_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/stl_extension/inc/resize.hpp"
#include "cieutils/packages/stl_extension/inc/make_shared_from_tuple.hpp"

// --- Internal Includes ---
#include "cmake_variables.hpp"

// --- STL Includes ---
#include <tuple>


namespace cie::csg {


template <  class CellType,
            class ValueType >
template <class ...Args>
SpaceTreeNode<CellType,ValueType>::SpaceTreeNode(   typename SpaceTreeNode<CellType,ValueType>::sampler_ptr p_sampler,
                                                    typename SpaceTreeNode<CellType,ValueType>::split_policy_ptr p_splitPolicy,
                                                    Size level,
                                                    Args&&... args ) :
    CellType( std::forward<Args>(args)... ),
    utils::AbsTree<std::vector,SpaceTreeNode<CellType,ValueType>>( level ),
    _p_splitPolicy( p_splitPolicy ),
    _p_sampler( p_sampler ),
    _isBoundary(-1)
{
}


template <  class CellType,
            class ValueType >
inline bool
SpaceTreeNode<CellType,ValueType>::divide(  const TargetFunction<typename CellType::point_type,ValueType>& r_target,
                                            Size level )
{
    // Evaluate target and set boundary flag
    evaluate( r_target );

    // Do nothing if this is the last level
    if ( this->_level >= level )
        return false;

    // Clear children
    this->_children.clear();

    // Split if boundary
    if ( _isBoundary == 1 )
    {
        auto splitPoint = _p_splitPolicy->operator()(
            _values.begin(),
            _values.end(),
            typename SpaceTreeNode<CellType,ValueType>::sample_point_iterator(0,*this)
        );

        auto nodeConstructor    = std::make_tuple(  _p_sampler,
                                                    _p_splitPolicy,
                                                    this->_level + 1 );
        auto p_cellConstructors = this->split( splitPoint );

        for ( const auto& cellConstructor : *p_cellConstructors )
        {
            // Construct a child
            auto compoundConstructor = std::tuple_cat(nodeConstructor,cellConstructor);
            auto p_node = utils::make_shared_from_tuple<SpaceTreeNode<CellType,ValueType>>(compoundConstructor);
            
            // Check whether child is valid
            if ( p_node->isDegenerate() )
                continue;

            this->_children.push_back(p_node);

            // Call divide on child
            p_node->divide( r_target, level );
        }

        return true;
    }

    return false;
}



template <  class CellType,
            class ValueType >
inline void
SpaceTreeNode<CellType,ValueType>::evaluate( const TargetFunction<typename CellType::point_type,ValueType>& r_target )
{
    _isBoundary = -1;
    cie::utils::resize( _values, _p_sampler->size() );
    auto it_value = _values.begin();
    typename SpaceTreeNode<CellType,ValueType>::sample_point_iterator it_point(0,*this);

    // Evaluate first point separately and set sign flag
    *it_value = r_target(*it_point++);
    bool isFirstValuePositive = *it_value > 0;
    it_value++;

    // Evaluate the rest of the points
    for ( ; it_value!=_values.end(); ++it_value,++it_point )
    {
        *it_value = r_target(*it_point);

        // Boundary check
        if ( ((*it_value>0) != isFirstValuePositive) && (_isBoundary < 0)  )
            _isBoundary = 1;
    }

    // If the boundary flag hasn't been modified -> not a boundary
    if ( _isBoundary < 0 )
        _isBoundary = 0;
}



template <  class CellType,
            class ValueType >
inline void
SpaceTreeNode<CellType,ValueType>::clear()
{
    // Clear data
    _values.clear();
    for ( auto& p_child : this->_children )
        p_child->clear();
    
    // Reset flags
    _isBoundary = -1;
}



template <  class CellType,
            class ValueType >
inline bool
SpaceTreeNode<CellType,ValueType>::isBoundary() const
{
    if ( _isBoundary < 0 )
        CIE_THROW( std::runtime_error, "SpaceTreeNode::isBoundary expects the node to be evaluated" )

    return _isBoundary==0 ? false : true;
}



template <  class CellType,
            class ValueType >
inline void
SpaceTreeNode<CellType,ValueType>::setSplitPolicy( typename SpaceTreeNode<CellType,ValueType>::split_policy_ptr p_splitPolicy )
{
    _p_splitPolicy = p_splitPolicy;
    this->_children.clear();
}



template <  class CellType,
            class ValueType >
inline void
SpaceTreeNode<CellType,ValueType>::setSampler( typename SpaceTreeNode<CellType,ValueType>::sampler_ptr p_sampler )
{
    // Set pointer
    _p_sampler = p_sampler;
    _values.clear();

    // Reset flags
    _isBoundary = -1;
}



template <  class CellType,
            class ValueType >
inline const typename SpaceTreeNode<CellType,ValueType>::split_policy_ptr&
SpaceTreeNode<CellType,ValueType>::splitPolicy() const
{
    return _p_splitPolicy;
}



template <  class CellType,
            class ValueType >
inline const typename SpaceTreeNode<CellType,ValueType>::value_container_type&
SpaceTreeNode<CellType,ValueType>::values() const
{
    return _values;
}



template <  class CellType,
            class ValueType >
inline const typename SpaceTreeNode<CellType,ValueType>::sampler_ptr&
SpaceTreeNode<CellType,ValueType>::sampler() const
{
    return _p_sampler;
}




/*
// Initialize static index converters
template <size_t N, size_t M>
SpaceTreeIndexConverter<N,M> SpaceTreeNode<N,M>::_dataIndex = SpaceTreeIndexConverter<N,M>();

template <size_t N, size_t M>
SpaceTreeIndexConverter<N,2> SpaceTreeNode<N,M>::_centerIndex = SpaceTreeIndexConverter<N,2>();


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
*/

} // namespace cie::csg

#endif