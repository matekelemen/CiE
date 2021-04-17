#ifndef CIE_CSG_TREES_CORNER_SAMPLE_HPP
#define CIE_CSG_TREES_CORNER_SAMPLE_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/partial_specialization.hpp"

// --- Internal Includes ---
#include "CSG/packages/trees/inc/PrimitiveSampler.hpp"
#include "CSG/packages/primitives/inc/Box.hpp"
#include "CSG/packages/primitives/inc/Cube.hpp"


namespace cie::csg {


CIE_DEFINE_INVALID_CLASS_TEMPLATE_TO_SPECIALIZE( CornerSampler )


template <concepts::Cube PrimitiveType>
class CornerSampler<PrimitiveType> : public PrimitiveSampler<PrimitiveType>
{
public:
    CornerSampler();

    virtual typename PrimitiveType::point_type getSamplePoint( const PrimitiveType& r_primitive,
                                                               Size index ) const override;

    virtual Size size() const override;

protected:
    CartesianIndexConverterPtr<PrimitiveType::dimension> _p_indexConverter;
};


template <concepts::Box PrimitiveType>
class CornerSampler<PrimitiveType> : public PrimitiveSampler<PrimitiveType>
{
public:
    CornerSampler();

    virtual typename PrimitiveType::point_type getSamplePoint( const PrimitiveType& r_primitive,
                                                               Size index ) const override;

    virtual Size size() const override;

protected:
    CartesianIndexConverterPtr<PrimitiveType::dimension> _p_indexConverter;
};


} // namespace cie::csg

#include "CSG/packages/trees/impl/CornerSampler_impl.hpp"

#endif