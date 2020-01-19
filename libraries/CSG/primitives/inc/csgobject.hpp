#ifndef CSG_CSG_OBJECT_HPP
#define CSG_CSG_OBJECT_HPP

#include "linalgtypes.hpp"

namespace cie {
namespace csg {


template <size_t N>
class CSGObject
{
public:
    virtual double operator()(const DoubleArray<N>& point) const = 0; 
};


}
}


#endif