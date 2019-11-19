#include "../inc/csgobject.hpp"

namespace csg {


bool CSGObject::isInside(const Point64& point) const {
        throw std::runtime_error("Calling virtual function CSGObject::isInside");
        return false;
    }


}