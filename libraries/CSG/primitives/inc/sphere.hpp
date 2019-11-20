#ifndef CSG_SPHERE_HPP
#define CSG_SPHERE_HPP

#include "csgobject.hpp"
#include "math.h"


namespace cie {
namespace csg {



class Sphere : public CSGObject {
private:
    double radius2_;
    Point64 center_;
public:
    Sphere(double radius, const Point64& center);
    Sphere(double radius);
    Sphere();

    bool isInside(const Point64& point) const;

    double getRadius() {return sqrt(radius2_);}
    const Point64& getCenter() {return center_;}

    void setRadius(double radius) {radius2_=radius*radius;}
    void setCenter(const Point64& center) {center_=center;}
};



}
}
#endif