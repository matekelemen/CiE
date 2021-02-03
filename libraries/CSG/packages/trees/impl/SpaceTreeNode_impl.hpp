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
SpaceTreeNode<CellType,ValueType>::divide(  const typename SpaceTreeNode<CellType,ValueType>::target_function& r_target,
                                            Size level )
{
    CIE_BEGIN_EXCEPTION_TRACING

    mp::ThreadPool pool;
    bool result = this->divide_internal(
        r_target,
        level,
        pool
    );
    pool.terminate();

    return result;

    CIE_END_EXCEPTION_TRACING
}


template <  class CellType,
            class ValueType >
inline bool
SpaceTreeNode<CellType,ValueType>::divide(  const typename SpaceTreeNode<CellType,ValueType>::target_function& r_target,
                                            Size level,
                                            typename SpaceTreeNode<CellType,ValueType>::target_map_ptr p_targetMap )
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Create target map if it doesn't exist yet
    if ( p_targetMap == nullptr )
        p_targetMap = typename SpaceTreeNode<CellType,ValueType>::target_map_ptr(
            new typename SpaceTreeNode<CellType,ValueType>::target_map_type
        );

    // Evaluate target and set boundary flag
    evaluateMap( r_target, p_targetMap );

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
            #pragma omp task
            {
            p_node->divide( r_target, level, p_targetMap );
            }
        }

        return true;
    }

    return false;

    CIE_END_EXCEPTION_TRACING
}



template <  class CellType,
            class ValueType >
inline void
SpaceTreeNode<CellType,ValueType>::evaluate( const typename SpaceTreeNode<CellType,ValueType>::target_function& r_target )
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Init
    _isBoundary = -1;
    cie::utils::resize( _values, _p_sampler->size() );
    auto it_value = _values.begin();
    typename SpaceTreeNode<CellType,ValueType>::sample_point_iterator it_point(0,*this);

    // Evaluate first point separately and set sign flag
    *it_value = r_target(*it_point++);
    bool isFirstValuePositive = *it_value > 0;
    it_value++;

    auto it_valueEnd = _values.end();

    // Evaluate the rest of the points
    for ( ; it_value!=it_valueEnd; ++it_value,++it_point )
    {
        *it_value = r_target(*it_point);

        // Boundary check
        if ( ((*it_value>0) != isFirstValuePositive) && (_isBoundary < 0)  )
            _isBoundary = 1;
    }

    // If the boundary flag hasn't been modified -> not a boundary
    if ( _isBoundary < 0 )
        _isBoundary = 0;

    CIE_END_EXCEPTION_TRACING
}



template <  class CellType,
            class ValueType >
inline typename SpaceTreeNode<CellType,ValueType>::target_map_ptr
SpaceTreeNode<CellType,ValueType>::evaluateMap( const TargetFunction<typename CellType::point_type,value_type>& r_target,
                                                typename SpaceTreeNode<CellType,ValueType>::target_map_ptr p_targetMap )
{
    CIE_BEGIN_EXCEPTION_TRACING

    CIE_CHECK_POINTER( p_targetMap )

    // Init
    _isBoundary = -1;
    cie::utils::resize( _values, _p_sampler->size() );
    auto it_value = _values.begin();
    typename SpaceTreeNode<CellType,ValueType>::sample_point_iterator it_point(0,*this);

    // Evaluate first point separately and set sign flag
    auto p_point      = &*it_point;
    auto it_targetMap = p_targetMap->find( *p_point );

    if ( it_targetMap == p_targetMap->end() )
    {
        it_targetMap = p_targetMap->emplace(
            *p_point,
            r_target( *p_point )
        ).first;
    }

    *it_value = it_targetMap->second;
    bool isFirstValuePositive = *it_value > 0;

    it_point++;
    it_value++;

    auto it_valueEnd = _values.end();

    // Evaluate the rest of the points
    for ( ; it_value!=it_valueEnd; ++it_value,++it_point )
    {
        p_point      = &*it_point;
        it_targetMap = p_targetMap->find( *p_point );

        if ( it_targetMap == p_targetMap->end() )
        {
            it_targetMap = p_targetMap->emplace(
                *p_point,
                r_target( *p_point )
            ).first;
        }

        *it_value = it_targetMap->second;

        // Boundary check
        if ( ((*it_value>0) != isFirstValuePositive) && (_isBoundary < 0)  )
            _isBoundary = 1;
    }

    // If the boundary flag hasn't been modified -> not a boundary
    if ( _isBoundary < 0 )
        _isBoundary = 0;

    return p_targetMap;

    CIE_END_EXCEPTION_TRACING
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


template <  class CellType,
            class ValueType >
inline bool
SpaceTreeNode<CellType,ValueType>::divide_internal( const typename SpaceTreeNode<CellType,ValueType>::target_function& r_target,
                                                    Size level,
                                                    mp::ThreadPool& r_pool )
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Clear children
    this->_children.clear();

    // Evaluate target and set boundary flag
    evaluate( r_target );

    // Do nothing if this is the last level
    if ( this->_level >= level )
        return false;

    // Split if boundary
    if ( this->_isBoundary == 1 )
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

            // Schedule divide on child
            r_pool.queueJob( [p_node,r_target,&r_pool,level]() -> void
                { p_node->divide_internal(r_target, level, r_pool); }
            );
        }

        return true;
    }

    return false;

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::csg

#endif