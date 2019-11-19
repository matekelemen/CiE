#ifndef CSG_CSGOBJECT_HPP
#define CSG_CSGOBJECT_HPP

#include "../../inc/csgtypes.hpp"

namespace csg{

class CSGObject {
public:
    virtual ~CSGObject(){}
    virtual bool isInside(const Point64& point) const;
};


}
#endif