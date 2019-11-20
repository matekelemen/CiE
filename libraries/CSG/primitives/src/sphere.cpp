#include "../inc/sphere.hpp"


namespace cie {
namespace csg {

Sphere::Sphere(double radius, const Point64& center) :
    radius2_(radius*radius),
    center_(center)
{
}

Sphere::Sphere(double radius) :
    radius2_(radius*radius),
    center_(Point64({0.0,0.0,0.0}))
{
}

Sphere::Sphere() :
    radius2_(1.0),
    center_(Point64({0.0,0.0,0.0}))
{
}




bool Sphere::isInside(const Point64& point) const {
    double norm = 0.0;
    for (uint i=0;i<point.size();++i){
        norm += (point[i]-center_[i]) * (point[i]-center_[i]);
    }
    return norm<=radius2_ ? true : false;
}


}
}