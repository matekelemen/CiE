#ifndef CIE_CSG_PRIMITIVE_SAMPLER_HPP
#define CIE_CSG_PRIMITIVE_SAMPLER_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/basic_concepts.hpp"

// --- Internal Includes ---
#include "CSG/packages/primitives/inc/concepts.hpp"
#include "CSG/packages/trees/inc/CartesianIndexConverter.hpp"


namespace cie::csg {


/// Interface for computing sample points on primitives.
template <concepts::Primitive PrimitiveType>
class PrimitiveSampler :
    public CSGTraits<PrimitiveType::dimension,typename PrimitiveType::coordinate_type>
{
public:
    using primitive_type = PrimitiveType;

public:
    virtual ~PrimitiveSampler() {}

    virtual typename PrimitiveType::point_type getSamplePoint( const PrimitiveType& r_primitive, Size index ) const = 0;
    virtual Size size() const = 0;
};


template <class PrimitiveType>
using PrimitiveSamplerPtr   = std::shared_ptr<PrimitiveSampler<PrimitiveType>>;


} // namespace cie::csg

#endif