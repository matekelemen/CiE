#ifndef CIE_CIEUTILS_ABS_TREE_HPP
#define CIE_CIEUTILS_ABS_TREE_HPP

// --- Internal Includes ---
#include "cieutils/packages/types/inc/types.hpp"

// --- STL Includes ---
#include <functional>
#include <memory>

namespace cie::utils {


// Define visitor function
template <class NodeType>
using NodeVisitFunction = std::function<bool( NodeType* node )>;



// Tree base class
template <template <class ...> class ContainerType, class SelfType, class ...Args>
class AbsTree
{
public:
    typedef SelfType                                    self_type;
    typedef std::shared_ptr<SelfType>                   self_pointer_type;
    typedef ContainerType<self_pointer_type,Args...>    child_container_type;

public:
    AbsTree( Size level );
    AbsTree();

    const child_container_type& children() const;
    child_container_type& children();

    const self_pointer_type child( Size index ) const;
    self_pointer_type child( Size index );

    Size level() const;

    // Send a function down the tree and execute it on all nodes while it returns true
    virtual bool visit( NodeVisitFunction<SelfType> function );

    /**
     * Check whether this node is a leaf node
     */
    virtual bool isLeaf() const;

protected:
    child_container_type    _children;
    Size                    _level;
};


}

#include "cieutils/packages/trees/impl/abstree_impl.hpp"

#endif