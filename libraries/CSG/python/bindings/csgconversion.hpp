#ifndef CIE_CSG_CSG_CONVERSION_CPP
#define CIE_CSG_CSG_CONVERSION_CPP

// --- Internal Includes ---
#include "../../packages/trees/inc/spacetreeutils.hpp"


namespace cie::csg {

template <uint8_t N>
using CellGeometry = std::array<DoubleVector,N+1>;


template <uint8_t N, uint8_t M>
CellGeometry<N> boundaryCells(const SpaceTreeNode<N,M>& root)
{
    auto nodes = boundaryNodes(root);
    CellGeometry<N> cells;

    for (size_t i=0; i<nodes.size(); ++i)
    {
        cells[0][i] = nodes[0]->edgeLength();
        for (uint8_t j=0; j<N; ++j)
            cells[j+1][i] = nodes[i]->center()[j];
    }

    return cells;
}


}

#endif