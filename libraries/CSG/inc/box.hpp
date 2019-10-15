#ifndef CSG_BOX_HPP
#define CSG_BOX_HPP

#include "csgobject.hpp"

namespace csg {

class Box : public CSGObject {
private:
    Point64 p0_;
    Point64 p1_;
public:
    Box();
    Box(const Box& box);
    Box(const Point64& p0, const Point64& p1);
    bool isInside(const Point64& point) const;

    void setPoint(uint index, Point64 newPoint);
    Point64 getPoint(uint index);

    Box& operator=(const Box& box);
};

}

#endif