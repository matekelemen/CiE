#ifndef DROPMERGE_DYNAMIC_TREE_HPP
#define DROPMERGE_DYNAMIC_TREE_HPP

// --- Internal Includes ---
#include "typedefs.hpp"
#include "mergefunction.hpp"





namespace cie::csg {



class DynamicTree : public NodeType
{
public:
    DynamicTree(    const PointType& base,
                    CoordinateType edgeLength,
                    Size level );

    void divide(size_t level);
    void offset(double value);

protected:
    DropFunction        _drop;
    GeometryFunction    _geometry;
    double              _offset;
};


} // namespace cie::csg

#endif