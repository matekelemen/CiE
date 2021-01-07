#ifndef CIE_DROP_MERGE_TYPE_DEFS_HPP
#define CIE_DROP_MERGE_TYPE_DEFS_HPP

// --- CSG Includes ---
#include <csg/trees.hpp>

// --- Utility Includes ---
#include <cieutils/types.hpp>

// --- STL Includes ---
#include <memory>


namespace cie::csg {



// Instantiate templates
const Size Dimension   = 3;
using CoordinateType   = Double;
using ValueType        = Double;
using PointType        = typename CSGTraits<Dimension,CoordinateType>::point_type;

using PrimitiveType    = Box<Dimension,CoordinateType>;
using CellType         = BoxCell<PrimitiveType>;
using NodeType         = SpaceTreeNode<CellType,ValueType>;
using NodePtr          = std::shared_ptr<NodeType>;

using SamplerType      = CartesianGridSampler<PrimitiveType>;
using SplitterType     = WeightedSplitPolicy< typename NodeType::sample_point_iterator,
                                              typename NodeType::value_iterator >;

using DropFunction     = std::function<ValueType(const PointType&, Double)>;
using GeometryFunction = TargetFunction<PointType,ValueType>;


} // namespace cie::csg

#endif