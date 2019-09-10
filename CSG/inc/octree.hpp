#ifndef OCTREE_HPP
#define OCTREE_HPP

#include "csgobject.hpp"
#include <iostream>
#include <vector>
#include <fstream>

namespace csg{

// -----------------------------------------------------------
// NODE
// -----------------------------------------------------------
class Node {
protected:
    CSGObject*          geometry_;
    NodeData            data_;
    BoundingBox         box_;
    std::array<Node*,8> children_;
    uint                level_;
public:
    Node();
    Node(const Node& parent, uint index);
    ~Node();

    void divide(uint minLevel, uint maxLevel);
    void evaluateAll();

    const NodeData&             data() const;
    bool                        data(uint index) const;
    const BoundingBox&          box() const;
    double                      box(uint i, uint j) const;
    const std::array<Node*,8>   children() const;
    uint                        level() const;

    void setGeometry(CSGObject* geometry);
    void setBoundingBox(const BoundingBox& box);
};


// -----------------------------------------------------------
// FREE-FLOATING FUNCTIONS
// -----------------------------------------------------------
bool isBoundary(const NodeData& data);
void interiorPoints(const Node& root,std::vector<Point64>& points);

std::array<bool,3>  base2(uint index);
std::array<uint,3>  base3(uint index);
uint                uintPow(uint base, uint exponent);
    
}




#endif