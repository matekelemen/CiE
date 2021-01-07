#ifndef CIE_CSG_TREES_CARTESIAN_GRID_SAMPLER_HPP
#define CIE_CSG_TREES_CARTESIAN_GRID_SAMPLER_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/partial_specialization.hpp"

// --- Internal Includes ---
#include "CSG/packages/trees/inc/PrimitiveSampler.hpp"
#include "CSG/packages/primitives/inc/Box.hpp"
#include "CSG/packages/primitives/inc/Cube.hpp"


namespace cie::csg {


/// Interface for cartesian samplers
template <concepts::Primitive PrimitiveType>
class AbsCartesianGridSampler : public PrimitiveSampler<PrimitiveType>
{
public:
    AbsCartesianGridSampler( Size numberOfPointsPerDimension );

    virtual Size size() const override;
    Size numberOfPointsPerDimension() const;
    void setNumberOfPointsPerDimension( Size numberOfPointsPerDimension );

protected:
    const CartesianIndexConverter<PrimitiveType::dimension>& indexConverter() const;

private:
    CartesianIndexConverterPtr<PrimitiveType::dimension> _p_indexConverter;
    Size                                                 _numberOfPointsPerDimension;
    Size                                                 _size;
};


/// Invalid default template
CIE_DEFINE_INVALID_CLASS_TEMPLATE_TO_SPECIALIZE( CartesianGridSampler )


/// Cube sampler
template <concepts::Cube PrimitiveType>
class CartesianGridSampler<PrimitiveType> : public AbsCartesianGridSampler<PrimitiveType>
{
public:
    using typename AbsCartesianGridSampler<PrimitiveType>::point_type;

public:
    CartesianGridSampler( Size numberOfPointsPerDimension );

    virtual point_type getSamplePoint( const PrimitiveType& r_primitive,
                                       Size index ) const override;
};


/// Box sampler
template <concepts::Box PrimitiveType>
class CartesianGridSampler<PrimitiveType> : public AbsCartesianGridSampler<PrimitiveType>
{
public:
    using typename AbsCartesianGridSampler<PrimitiveType>::point_type;

public:
    CartesianGridSampler( Size numberOfPointsPerDimension );

    virtual point_type getSamplePoint( const PrimitiveType& r_primitive,
                                       Size index ) const override;
};


} // namespace cie::csg

#include "CSG/packages/trees/impl/CartesianGridSampler_impl.hpp"

#endif