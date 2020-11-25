#ifndef CIE_CSG_PARTITIONING_AABBOX_NODE_IMPL_HPP
#define CIE_CSG_PARTITIONING_AABBOX_NODE_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"


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
bool
AABBoxNode<ObjectType>::addObject( typename AABBoxNode<ObjectType>::object_ptr p_object )
{
    // TODO
    return false;
}


template <concepts::BoxBoundable ObjectType>
void
AABBoxNode<ObjectType>::shrink()
{
    // TODO
}


template <concepts::BoxBoundable ObjectType>
typename AABBoxNode<ObjectType>::self_ptr
AABBoxNode<ObjectType>::find( const typename AABBoxNode<ObjectType>::object_ptr p_object )
{
    // TODO
    return this->weak_from_this();
}


template <concepts::BoxBoundable ObjectType>
bool
AABBoxNode<ObjectType>::partition( Size maxObjects,
                                   Size maxLevel )
{
    // TODO
    return false;
}


template <concepts::BoxBoundable ObjectType>
inline const typename AABBoxNode<ObjectType>::object_ptr_container&
AABBoxNode<ObjectType>::objects() const
{
    return this->_objects();
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