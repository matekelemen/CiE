
// --- Internal Includes --
#include "../inc/dynamictree.hpp"

namespace cie::csg {


DynamicTree::DynamicTree(   const PointType& base, 
                            CoordinateType edgeLength,
                            Size level ) :
    NodeType(
        typename NodeType::sampler_ptr( new SamplerType(numberOfPointsPerDimension) ),
        typename NodeType::split_policy_ptr( new SplitterType ),
        level,
        base,
        edgeLength
    ),
    _drop(exponentialMergeFunction<Dimension>),
    _offset(0.0)
{
    _geometry = [this](const PointType& r_point)
                    {return this->_drop(r_point,_offset);};
}


void DynamicTree::divide(size_t level)
{
    NodeType::divide(_geometry, level);
}


void DynamicTree::offset(double value)
{
    NodeType::clear();
    _offset = value;
}


} // namespace cie::csg
