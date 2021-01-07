#ifndef CIE_CSG_SPACE_TREE_NODE_HPP
#define CIE_CSG_SPACE_TREE_NODE_HPP

// --- Linalg Includes ---
#include "linalg/packages/types/inc/arraytypes.hpp"

// --- Utility Includes ---
#include "cieutils/packages/trees/inc/abstree.hpp"
#include "cieutils/packages/concurrency/inc/ThreadSafeMap.hpp"

// --- Internal Includes ---
#include "CSG/packages/trees/inc/AbsCell.hpp"
#include "CSG/packages/trees/inc/SplitPolicy.hpp"
#include "CSG/packages/trees/inc/CartesianIndexConverter.hpp"

// --- STL Includes ---
#include <deque>
#include <stdint.h>
#include <memory>
#include <functional>
#include <map>

namespace cie::csg {


/* --- TargetFunction --- */

template <  concepts::NumericContainer PointType,
            class ValueType >
using TargetFunction = std::function<ValueType(const PointType&)>;



/* --- SpaceTreeNode --- */

template <  class CellType,
            class ValueType >
class SpaceTreeNode :
    public CellType,
    public utils::AbsTree<std::vector,SpaceTreeNode<CellType,ValueType>>
{
public:
    /**
     * Point iterator that lazily generates sample points. 
    */
    struct sample_point_iterator
    {
        typedef typename SpaceTreeNode::point_type  value_type;
        typedef int                                 difference_type;
        typedef value_type*                         pointer;
        typedef value_type&                         reference;
        typedef std::random_access_iterator_tag     iterator_category;

        sample_point_iterator( Size counter, const SpaceTreeNode<CellType,ValueType>& r_node ) :
            _counter(counter),
            _r_node( r_node )
        { **this; }
        sample_point_iterator& operator++()                         { ++_counter; return *this; };
        sample_point_iterator& operator++(int)                      { _counter++; return *this; }
        sample_point_iterator& operator--()                         { --_counter; return *this; }
        sample_point_iterator& operator--(int)                      { _counter--; return *this; }
        sample_point_iterator& operator+=( Size offset )            { _counter += offset; return *this; }
        sample_point_iterator& operator-=( Size offset )            { _counter -= offset; return *this; }
        const value_type& operator*()                               { updatePoint(); return _point; }
        value_type const* operator->()                              { updatePoint(); return &_point; }
        bool operator!=( const sample_point_iterator& r_rhs )       { return this->_counter != r_rhs._counter;}

    protected:
        void updatePoint()                                          { _point = _r_node._p_sampler->getSamplePoint(_r_node,_counter); }

    private:
        Size                                        _counter;
        const SpaceTreeNode<CellType,ValueType>&    _r_node;
        value_type                                  _point;
    };

public:
    using cell_type             = CellType;
    using value_type            = ValueType;
    using value_container_type  = std::vector<value_type>;
    using value_iterator        = typename value_container_type::const_iterator;

    using sampler_ptr           = PrimitiveSamplerPtr<typename CellType::primitive_type>;
    using split_policy_ptr      = SplitPolicyPtr<sample_point_iterator,value_iterator>;

    using target_map_base_type  = std::map<typename cell_type::point_type, value_type>;
    using target_map_type       = mp::ThreadSafeMap<target_map_base_type>;
    using target_map_ptr        = std::shared_ptr<target_map_type>;

public:
    /**
     * Constructor that forwards its arguments to the 
     * constructor of its primitive (through the cell constructor).
    */
    template <class ...Args>
    SpaceTreeNode(  sampler_ptr p_sampler,
                    split_policy_ptr p_splitPolicy,
                    Size level,
                    Args&&... args );

    /**
     * Evaluate the target function at all sample points and split the
     * node if the results have mixed signs.
    */
    bool divide(    const TargetFunction<typename CellType::point_type,value_type>& r_target,
                    Size level );

    /**
     * Evaluate the target function at all sample points, store the results in a map,
     * and split the node if the results have mixed signs.
    */
    bool divide(    const TargetFunction<typename CellType::point_type,value_type>& r_target,
                    Size level,
                    target_map_ptr p_targetMap );

    /**
     * Evaluate the target function at all sample points and store the results.
    */ 
    virtual void evaluate( const TargetFunction<typename CellType::point_type,value_type>& r_target );

    /**
     * Alternative to evaluate.
     * Record sample points and their values in a global map, then set the isBoundary flag.
     * 
     * Note: insane overhead; probably only worth it if the target function is very expensive
    */
    target_map_ptr evaluateMap( const TargetFunction<typename CellType::point_type,value_type>& r_target,
                                target_map_ptr p_targetMap = nullptr );

    /// Clear data container and call clear on children.
    void clear();

    /**
     * Check whether this cell is cut.
     * The node needs to be in an evaluated state, otherwise
     * an exception is thrown.
    */
    bool isBoundary() const;

    void setSplitPolicy( split_policy_ptr p_splitPolicy );
    void setSampler( sampler_ptr p_sampler );

    const split_policy_ptr& splitPolicy() const;
    const value_container_type& values() const;
    const sampler_ptr& sampler() const;

protected:
    split_policy_ptr        _p_splitPolicy;

private:
    value_container_type    _values;
    sampler_ptr             _p_sampler;
    int8_t                  _isBoundary; // 1:true 0:false -1:unevaluated
};



template <class CellType, class ValueType>
using SpaceTreeNodePtr = std::shared_ptr<SpaceTreeNode<CellType,ValueType>>;


} // namespace cie::csg

#include "CSG/packages/trees/impl/SpaceTreeNode_impl.hpp"

#endif