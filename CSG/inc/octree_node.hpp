#ifndef CSG_OCTREE_NODE_HPP
#define CSG_OCTREE_NODE_HPP

#include "csgtypes.hpp"
#include "octreeutils.hpp"

namespace csg {

class Node {
protected:
    BoundingBox         box_;
    std::array<Node*,8> children_;
    uint                level_;
    const Node*         parent_;
public:
    // CONSTRUCTORS -------------------------------------
    Node();
    Node(const Node& parent, uint index);
    virtual ~Node(){};
    // 
    virtual void divide(uint minLevel, uint maxLevel) =0;   
    // SET/GET ------------------------------------------
    const BoundingBox& box() const;
    double box(uint i, uint j) const;
    const std::array<Node*,8>& children() const;
    uint level() const;
    const Node* parent() const;
    void setBoundingBox(const BoundingBox& box);
};

}


#endif