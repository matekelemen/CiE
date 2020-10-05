#ifndef DROPMERGE_COLLECT_NODES_HPP
#define DROPMERGE_COLLECT_NODES_HPP

// --- Linalg Includes ---
#include <linalg/types.hpp>

// --- Internal Includes ---
#include "dynamictree.hpp"


namespace cie::csg {


class TreeData
{
public:
    TreeData();
    std::vector<PointType>& centers() { return _centers; }
    DoubleVector& edgeLengths() { return _edgeLengths; }
    std::vector<int>& boundaries() { return _boundaries; }

private:
    std::vector<PointType>  _centers;
    DoubleVector            _edgeLengths;
    std::vector<int>        _boundaries;
};


TreeData collectNodes(const DynamicTree& r_root);

void collectNodesRecursive(     const NodeType& r_root,
                                std::vector<PointType>& r_centers,
                                DoubleVector& r_edgeLengths,
                                std::vector<int>& r_boundaries);


} // namespace cie::csg

#endif