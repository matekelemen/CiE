#ifndef CSG_OCTREE_UTILS_HPP
#define CSG_OCTREE_UTILS_HPP

#include "csgtypes.hpp"
#include <utility>

namespace csg{

// General octree node helpers
std::array<bool,3>  base2(uint index);
std::array<uint,3>  base3(uint index);
uint                uintPow(uint base, uint exponent);

}

#endif