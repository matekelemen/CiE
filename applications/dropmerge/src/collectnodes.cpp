// --- Internal Includes ---
#include "../inc/collectnodes.hpp"

namespace cie::csg {


TreeData::TreeData() :
    _centers(),
    _edgeLengths(),
    _boundaries()
{
}


TreeData collectNodes(const DynamicTree& root)
{
    TreeData data;

    collectNodesRecursive(  root,
                            data.centers(),
                            data.edgeLengths(),
                            data.boundaries()    );
    return data;
}


void collectNodesRecursive(     const NodeType& r_root,
                                std::vector<PointType>& r_centers,
                                DoubleVector& r_edgeLengths,
                                std::vector<int>& r_boundaries)
{
    auto center     = r_root.base();
    auto edgeLength = r_root.length();

    for ( auto& component : center )
        component += edgeLength / 2.0;

    r_centers.push_back( center );
    r_edgeLengths.push_back( edgeLength );
    r_boundaries.push_back( r_root.isBoundary() );

    for ( auto& rp_child : r_root.children() )
    {
        if ( rp_child != nullptr )
            collectNodesRecursive(  *rp_child,
                                    r_centers,
                                    r_edgeLengths,
                                    r_boundaries);
        else break;
    }
}


} // namespace cie::csg