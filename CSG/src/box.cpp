#include "box.hpp"
#include <iostream>

namespace csg {


Box::Box() : 
    p0_(0.0),
    p1_(0.0)
{
}

Box::Box(const Box& box) :
    p0_(box.p0_),
    p1_(box.p1_)
{
}

Box::Box(const Point64& p0, const Point64& p1) : 
    CSGObject(),
    p0_(p0),
    p1_(p1)
{
}


bool Box::isInside(const Point64& point) const {

    for (uint i=0;i<point.size();++i){
        if ( sgn(point[i]-p0_[i]) == sgn(point[i]-p1_[i])) return false;
    }
    return true;
}


void Box::setPoint(uint index, Point64 newPoint){
    index==0 ? p0_=newPoint : p1_=newPoint;
}

Point64 Box::getPoint(uint index){
    return index==0 ? p0_ : p1_;
}




Box& Box::operator=(const Box& box){
    Box* newBox = new Box(p0_, p1_);
    return *newBox;
}



}