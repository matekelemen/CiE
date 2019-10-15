#ifndef OCTREE_VERTEXNODE_HPP
#define OCTREE_VERTEXNODE_HPP

#include "octree_node.hpp"
#include "csgobject.hpp"
#include <iostream>
#include <vector>
#include <fstream>

namespace csg{


class VertexNode : public Node {
protected:
    CSGObject*          geometry_;
    NodeData            data_;
public:
    // Constructors ------------------------------------------
    VertexNode();
    VertexNode(const VertexNode& parent, uint index);
    ~VertexNode();
    // Base override -----------------------------------------
    void divide(uint minLevel, uint maxLevel);
    // Calc --------------------------------------------------
    void evaluateAll();
    // Set/Get -----------------------------------------------
    const NodeData& data() const;
    bool data(uint index) const;
    void setGeometry(CSGObject* geometry);
    // Search ------------------------------------------------
    void interiorPoints(std::vector<Point64>& points);
};



// Free-floating functions
Point64 getNodePoint(const VertexNode& root, uint index);
bool isBoundary(const NodeData& data);
std::pair<const VertexNode&, uint> getNeighbour(const VertexNode& root, uint index, uint axis, bool direction);



    
}




#endif