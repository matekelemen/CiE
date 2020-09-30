#ifndef CIE_CSG_SPACE_TREE_NODE_HPP
#define CIE_CSG_SPACE_TREE_NODE_HPP

// --- Linalg Includes ---
#include "linalg/packages/types/inc/arraytypes.hpp"

// --- Utility Includes ---
#include "cieutils/packages/trees/inc/abstree.hpp"

// --- Internal Includes ---
#include "CSG/packages/trees/inc/cell.hpp"
#include "CSG/packages/trees/inc/split_policy.hpp"

// --- STL Includes ---
#include <deque>
#include <stdint.h>
#include <memory>
#include <functional>

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
    struct sample_point_iterator
    {
        typedef typename SpaceTreeNode::point_type  value_type;
        typedef Size                                difference_type;
        typedef value_type*                         pointer;
        typedef value_type&                         reference; 

        sample_point_iterator( Size counter ) : _counter(counter)   {}
        sample_point_iterator() : sample_point_iterator(0)          {}
        sample_point_iterator& operator++()                         { ++_counter; return *this; };
        sample_point_iterator& operator++(int)                      { _counter++; return *this; }
        sample_point_iterator& operator--()                         { --_counter; return *this; }
        sample_point_iterator& operator--(int)                      { _counter--; return *this; }
        sample_point_iterator& operator+=( Size offset )            { _counter += offset; return *this; }
        sample_point_iterator& operator-=( Size offset )            { _counter -= offset; return *this; }
        value_type operator*()                                      { /* TODO */ return value_type(); };
        bool operator!=( sample_point_iterator rhs )                { return this->_counter != rhs._counter;}

    private:
        Size _counter;
    };

public:
    using cell_type             = CellType;
    using value_type            = ValueType;
    using value_container_type  = std::vector<value_type>;
    using value_iterator        = typename value_container_type::const_iterator;

    using split_policy_ptr      = SplitPolicyPtr<sample_point_iterator,value_iterator>;

public:

    /**
     * Constructor that forwards its arguments to the 
     * constructor of its primitive (through the cell constructor).
    */
    template <class ...Args>
    SpaceTreeNode(  PrimitiveSamplerPtr<CellType> p_sampler,
                    split_policy_ptr p_splitPolicy,
                    Size level,
                    Args&&... args );



protected:
    PrimitiveSamplerPtr<CellType>   _p_sampler;
    split_policy_ptr                _p_splitPolicy;
    value_container_type            _values;
};


/*
template <  class CellType,
            class SamplerType,
            class SplitPolicy,
            concepts::STLContainer ValueContainerType = std::vector<typename SplitPolicy::value_type> >
class SpaceTreeNode_ : public CellType
{
public:
    static const Size                                   resolution = SamplerType::resolution;

    typedef CellType                                    cell_type;
    typedef ValueContainerType                          value_container_type;
    typedef typename value_container_type::value_type   value_type;
    typedef SamplerType                                 sampler_type;

public:
    template <class ...Args>
    SpaceTreeNode_( Args&&... args );

    const value_container_type& values() const;
    value_container_type& values();
    const value_type& value( Size index ) const;
    value_type& value( Size index );

protected:
    value_container_type        _values;
    static const sampler_type   _sampler = sampler_type();

    
public: // Member classes

    struct sample_point_iterator
    {
        typedef typename SpaceTreeNode_::point_type value_type;

        sample_point_iterator( Size counter ) : _counter(counter) {}
        sample_point_iterator() : sample_point_iterator(0) {}
        sample_point_iterator& operator++();
        value_type operator*() {return };
        bool operator!=( sample_point_iterator rhs ) {return this->_counter != rhs._counter;}

    private:
        Size _counter;
    };
};
*/

/*
// Define node pointer (needs forward declaration)
template <size_t N, size_t M>
class SpaceTreeNode;

template <size_t N, size_t M>
using SpaceTreeNodePtr = std::unique_ptr<SpaceTreeNode<N,M>>;



template <size_t N, size_t M>
class SpaceTreeNode {
public:
    typedef DoubleArray<N>
        coordinate_container_type;
    typedef DoubleArray<SpaceTreeIndexConverter<N,M>::_numberOfDataPoints>
        data_container_type;
    typedef std::array<SpaceTreeNodePtr<N,M>,SpaceTreeIndexConverter<N,M>::_numberOfChildren>
        child_container_type;

    SpaceTreeNode();
    SpaceTreeNode(  const coordinate_container_type& center, 
                    double edgeLength);
    SpaceTreeNode(  SpaceTreeNode<N, M>& parent,
                    size_t index,
                    const GeometryFunction<N>& geometry,
                    bool autoEvaluate = true);

    void evaluate(const GeometryFunction<N>& geometry, size_t index);
    void evaluate(const GeometryFunction<N>& geometry);
    
    bool divide(const GeometryFunction<N>& geometry, size_t level);
    bool divideOffload( const GeometryFunction<N>& geometry, size_t level );

    void write(std::ostream& file) const;
    void wipe();

    coordinate_container_type pointCoordinates(size_t index) const;
    coordinate_container_type pointCoordinates(const UIntArray<N>& indexN) const;

    coordinate_container_type& center();
    const coordinate_container_type& center() const;
    const data_container_type& data() const;
    const child_container_type& children() const;
    const SpaceTreeNode<N,M>& child(size_t index) const;
    double edgeLength() const;

protected:
    bool divideRecursive(const GeometryFunction<N>& geometry, size_t level);

    bool divideOffloadRecursive( const GeometryFunction<N>& geometry, size_t level );
    void requestEvaluation( SpaceTreeNode<N,M>& parent,
                            const coordinate_container_type& point, 
                            double* dataPtr );
    
    coordinate_container_type                                   _center;
    data_container_type                                         _data;
    child_container_type                                        _children;
    double                                                      _edgeLength;
    static SpaceTreeIndexConverter<N,M>                         _dataIndex;
    static SpaceTreeIndexConverter<N,2>                         _centerIndex;
    std::deque<std::pair<coordinate_container_type,double*>>    _evaluationRequests;
};
*/


} // namespace cie::csg

#include "CSG/packages/trees/impl/spacetreenode_impl.hpp"

#endif