#ifndef CIE_CSG_PRIMITIVES_PRIMITIVE_HPP
#define CIE_CSG_PRIMITIVES_PRIMITIVE_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/basic_concepts.hpp"

// --- Internal Includes ---
#include "CSG/packages/primitives/inc/csgobject.hpp"


namespace cie::csg {


template < Size Dimension,
           concepts::NumericType CoordinateType = Double >
class AbsPrimitive : public CSGTraits<Dimension,CoordinateType>
{
public:
    virtual Bool isDegenerate() const = 0;
};


} // namespace cie::csg

#endif