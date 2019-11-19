#ifndef CSG_VOLUMENODE_HPP
#define CSG_VOLUMENODE_HPP

#include "octree_node.hpp"

namespace csg {

class VolumeNode : public Node {
protected:
    std::vector<const Point64*> activePoints_;
public:
    VolumeNode();
    VolumeNode(const VolumeNode& parent, uint index);
    ~VolumeNode();

    const VolumeNode* locate(const Point64* target, double tolerance);
    void divide(uint minLevel, uint maxLevel);
    void divide(uint minLevel, uint maxLevel, uint maxPoints);

    const VolumeNode* findContainingNode(const Point64* target) const;

    bool isInside(const Point64& point) const;

    std::vector<const Point64*>& activePoints();
    const std::vector<const Point64*>& activePoints() const;
    void setPoints(const std::vector<Point64>& set);
};


}



#endif