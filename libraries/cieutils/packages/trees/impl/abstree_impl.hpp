#ifndef CIE_CIEUTILS_ABS_TREE_IMPL_HPP
#define CIE_CIEUTILS_ABS_TREE_IMPL_HPP

namespace cie::utils {


template <template <class ...> class ContainerType, class SelfType, class ...Args>
AbsTree<ContainerType,SelfType,Args...>::AbsTree( Size level ) :
    _level(level)
{
}


template <template <class ...> class ContainerType, class SelfType, class ...Args>
AbsTree<ContainerType,SelfType,Args...>::AbsTree() :
    _level(SIZE_MAX)
{
}


template <template <class ...> class ContainerType, class SelfType, class ...Args>
inline const typename AbsTree<ContainerType,SelfType,Args...>::child_container_type&
AbsTree<ContainerType,SelfType,Args...>::children() const
{
    return _children;
}


template <template <class ...> class ContainerType, class SelfType, class ...Args>
inline typename AbsTree<ContainerType,SelfType,Args...>::child_container_type&
AbsTree<ContainerType,SelfType,Args...>::children()
{
    return this->_children;
}


template <template <class ...> class ContainerType, class SelfType, class ...Args>
inline const typename AbsTree<ContainerType,SelfType,Args...>::self_pointer_type
AbsTree<ContainerType,SelfType,Args...>::child( Size index ) const
{
    return _children[index];
}


template <template <class ...> class ContainerType, class SelfType, class ...Args>
inline typename AbsTree<ContainerType,SelfType,Args...>::self_pointer_type
AbsTree<ContainerType,SelfType,Args...>::child( Size index )
{
    return this->_children[index];
}


template <template <class ...> class ContainerType, class SelfType, class ...Args>
inline Size
AbsTree<ContainerType,SelfType,Args...>::level() const
{
    return _level;
}


template <template <class ...> class ContainerType, class SelfType, class ...Args>
inline bool
AbsTree<ContainerType,SelfType,Args...>::visit( NodeVisitFunction<SelfType> function )
{
    bool result = function( dynamic_cast<SelfType*>(this) );

    if (result)
        for (auto& child : this->_children)
        {
            result = child->visit(function);
            if (!result)
                break;
        }
    
    return result;
}


template <template <class ...> class ContainerType, class SelfType, class ...Args>
inline bool
AbsTree<ContainerType,SelfType,Args...>::isLeaf() const
{
    // Has no children, or all children are nullptrs
    
    if ( this->_children.empty() )
        return true;

    for ( const auto& rp_child : this->_children )
        if ( rp_child )
            return false;

    return true;
}


} // namespace cie::utils

#endif