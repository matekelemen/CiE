#ifndef CIE_CSG_PARTITIONING_AABBOX_NODE_IMPL_HPP
#define CIE_CSG_PARTITIONING_AABBOX_NODE_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/stl_extension/inc/resize.hpp"

// --- STL Includes ---
#include <algorithm>
#include <limits>


namespace cie::csg {


template <concepts::BoxBoundable ObjectType>
AABBoxNode<ObjectType>::AABBoxNode( const typename AABBoxNode<ObjectType>::point_type& r_base,
                                    const typename AABBoxNode<ObjectType>::point_type& r_lengths,
                                    typename AABBoxNode<ObjectType>::self_ptr p_parent ) :
    BoxCell<typename ObjectType::bounding_box>( r_base, r_lengths ),
    utils::AbsTree<std::deque,AABBoxNode<ObjectType>>(),
    _objects(),
    _p_parent( p_parent )
{
    CIE_BEGIN_EXCEPTION_TRACING

    if ( auto p_tmp = _p_parent.lock() )
        this->_level = p_tmp->_level + 1;
    else
        this->_level = 0;

    CIE_END_EXCEPTION_TRACING
}


template <concepts::BoxBoundable ObjectType>
AABBoxNode<ObjectType>::AABBoxNode() :
    AABBoxNode<ObjectType>( detail::makeOrigin<ObjectType::bounding_box::dimension,typename ObjectType::bounding_box::coordinate_type>(),
                            detail::makeOrigin<ObjectType::bounding_box::dimension,typename ObjectType::bounding_box::coordinate_type>(),
                            typename AABBoxNode<ObjectType>::self_ptr() )
{
}


template <concepts::BoxBoundable ObjectType>
void
AABBoxNode<ObjectType>::addObject( typename AABBoxNode<ObjectType>::object_ptr p_object )
{
    auto nodeVisitFunction = [&p_object]( AABBoxNode<ObjectType>* p_node ) -> bool
    {
        CIE_BEGIN_EXCEPTION_TRACING

        if ( auto p_tmp = p_object.lock() )
            if ( p_node->contains( p_tmp->boundingBox() ) )
                p_node->_objects.push_back( p_tmp );

        return true;

        CIE_END_EXCEPTION_TRACING
    };

    this->visit( nodeVisitFunction );
}


template <concepts::BoxBoundable ObjectType>
void
AABBoxNode<ObjectType>::eraseExpired()
{
    auto nodeVisitFunction = []( AABBoxNode<ObjectType>* p_node ) -> bool
    {
        std::erase_if( p_node->_objects,
                       []( const auto& rp_object ) -> bool { return rp_object.expired(); } );
        return true;
    };
}


template <concepts::BoxBoundable ObjectType>
void
AABBoxNode<ObjectType>::shrink()
{
    auto nodeVisitFunction = []( AABBoxNode<ObjectType>* p_node ) -> bool
    {
        CIE_BEGIN_EXCEPTION_TRACING
        
        if ( p_node->_objects.empty() )
        {

            std::fill( p_node->_lengths.begin(),
                       p_node->_lengths.end(),
                       0 );
            return true;
        }

        typename AABBoxNode<ObjectType>::point_type minPoint;
        utils::resize( minPoint, AABBoxNode<ObjectType>::dimension );
        std::fill( minPoint.begin(),
                   minPoint.end(),
                   std::numeric_limits<typename AABBoxNode<ObjectType>::coordinate_type>().max() );

        typename AABBoxNode<ObjectType>::point_type maxPoint;
        utils::resize( maxPoint, AABBoxNode<ObjectType>::dimension );
        std::fill( maxPoint.begin(),
                   maxPoint.end(),
                   std::numeric_limits<typename AABBoxNode<ObjectType>::coordinate_type>().min() );

        for ( auto& rp_object : p_node->_objects )
        {
            if ( auto p_tmp = rp_object.lock() )
            {
                const auto& r_box = p_tmp->boundingBox();

                for ( Size dim=0; dim<AABBoxNode<ObjectType>::dimension; ++dim )
                {
                    auto boxMax = r_box.base()[dim] + r_box.lengths()[dim];

                    if ( r_box.base()[dim] < minPoint[dim] )
                        minPoint[dim] = r_box.base()[dim];

                    if ( maxPoint[dim] < boxMax )
                        maxPoint[dim] = boxMax;
                } // for dim
            } // lock object
        } // for object in objects

        for ( Size dim=0; dim<AABBoxNode<ObjectType>::dimension; ++dim )
        {
            p_node->_base[dim]    = minPoint[dim];
            p_node->_lengths[dim] = maxPoint[dim] - minPoint[dim];
        }

        return true;

        CIE_END_EXCEPTION_TRACING
    };

    this->visit( nodeVisitFunction );
}


template <concepts::BoxBoundable ObjectType>
typename AABBoxNode<ObjectType>::self_ptr
AABBoxNode<ObjectType>::find( const typename AABBoxNode<ObjectType>::object_ptr p_object )
{
    if ( p_object.expired() )
        CIE_THROW( Exception, "Attempt to find expired object in box tree" )

    auto p_objectInternal = p_object.lock();

    typename AABBoxNode<ObjectType>::self_ptr p_containingNode;
    Size maxLevel = 0;

    auto nodeVisitFunction = [&p_objectInternal,&p_containingNode,&maxLevel]( AABBoxNode<ObjectType>* p_node ) -> bool
    {
        CIE_BEGIN_EXCEPTION_TRACING

        if ( p_node->level() < maxLevel )
            return false;

        if ( std::find( p_node->objects().begin(),
                        p_node->objects().end(),
                        p_objectInternal )
             != p_node->objects().end() )
        {
            p_containingNode = p_node->weak_from_this();
            maxLevel = p_node->level();
        }

        return true;

        CIE_END_EXCEPTION_TRACING
    };

    // Check whether the object is in this box,
    // if it is not, there is no point in searching subcells
    if ( nodeVisitFunction(this) )
        this->visit( nodeVisitFunction );

    return p_containingNode;
}


template <concepts::BoxBoundable ObjectType>
bool
AABBoxNode<ObjectType>::partition( Size maxObjects,
                                   Size maxLevel )
{
    bool objectLimitReached = true;

    auto nodeVisitFunction = [=,&objectLimitReached]( AABBoxNode<ObjectType>* p_node ) -> bool
    {
        if ( auto p_parent = p_node->parent().lock() ) // Not root
        {
            p_node->_objects.clear();

            // Pull contained objects from the parent
            for ( auto& rp_object : p_parent->_objects )
                if ( auto p_tmp = rp_object.lock() )
                    if ( p_node->contains( p_tmp->boundingBox() ) )
                        p_node->_objects.push_back( p_tmp );
        }

        // Stop partitioning if the number of objects is within the limit
        if ( p_node->_objects.size() <= maxObjects )
            return true;

        // Stop partitioning if maximum recursion limit is reached
        if ( maxLevel <= p_node->_level )
        {
            objectLimitReached = false;
            return true;
        }

        // Get node midpoint to use as splitting point
        typename AABBoxNode<ObjectType>::point_type midPoint;
        utils::resize( midPoint, AABBoxNode<ObjectType>::dimension );
        for ( Size dim=0; dim<AABBoxNode<ObjectType>::dimension; ++dim )
            midPoint[dim] = p_node->_base[dim] + p_node->_lengths[dim] / 2.0;

        // Subdivide node
        auto p_cellConstructors = p_node->split( midPoint );

        for ( const auto& r_cellConstructor : *p_cellConstructors )
            p_node->_children.emplace_back( new AABBoxNode<ObjectType>(
                std::get<0>( r_cellConstructor ),   // base
                std::get<1>( r_cellConstructor ),   // lengths
                p_node->weak_from_this()            // parent
            ));

        return true;
    };

    this->visit( nodeVisitFunction );

    return objectLimitReached;
}


template <concepts::BoxBoundable ObjectType>
inline const typename AABBoxNode<ObjectType>::object_ptr_container&
AABBoxNode<ObjectType>::objects() const
{
    return this->_objects;
}


template <concepts::BoxBoundable ObjectType>
inline typename AABBoxNode<ObjectType>::self_ptr
AABBoxNode<ObjectType>::parent()
{
    return this->_p_parent;
}


template <concepts::BoxBoundable ObjectType>
inline const typename AABBoxNode<ObjectType>::self_ptr&
AABBoxNode<ObjectType>::parent() const
{
    return this->_p_parent;
}


} // namespace cie::csg

#endif