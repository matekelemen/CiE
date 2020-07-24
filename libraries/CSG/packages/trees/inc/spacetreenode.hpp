#ifndef CIE_CSG_NTREENODE_HPP
#define CIE_CSG_NTREENODE_HPP

// --- Linalg Includes ---
#include "linalg/types.hpp"

// --- Internal Includes ---
#include "./cell.hpp"
#include "./primitive_sampler.hpp"

// --- STL Includes ---
#include <deque>
#include <stdint.h>
#include <memory>
#include <functional>

namespace cie::csg {


// Target function definition
template <  concepts::NumericContainer PointType,
            class ValueType >
using TargetFunction = std::function<ValueType(const PointType&)>;


/*
template <  class CellType,
            class SamplerType,
            class SplitPolicy,
            concepts::STLContainer ValueContainerType = std::array<ValueType,SamplerType::size()> >
class SpaceTreeNode : public CellType
{
public:
    static const Size                           resolution = SamplerType::resolution;

    typedef CellType                            cell_type;
    typedef typename SamplerType::value_type    value_type;
    typedef ValueContainerType                  value_container_type;

public:
    SpaceTreeNode();

    const value_container_type& values() const;
    value_container_type& values();
    const value_type& value( Size index ) const;
    value_type& value( Size index );

protected:
    value_container_type    _values;
};
*/

///*
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
//*/


}

#include "../impl/spacetreenode_impl.hpp"

#endif