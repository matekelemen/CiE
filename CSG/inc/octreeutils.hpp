#ifndef CSG_OCTREE_UTILS_HPP
#define CSG_OCTREE_UTILS_HPP

#include "octree.hpp"
#include <utility>

namespace csg{

class Node;

Point64 getNodePoint(const Node& root, uint index);

bool isBoundary(const NodeData& data);
void interiorPoints(const Node& root,std::vector<Point64>& points);

std::pair<const Node&, uint> getNeighbour(const Node& root, uint index, uint axis, bool direction);

std::array<bool,3>  base2(uint index);
std::array<uint,3>  base3(uint index);
uint                uintPow(uint base, uint exponent);

}

#endif