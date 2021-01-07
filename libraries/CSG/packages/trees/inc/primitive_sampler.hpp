#ifndef CIE_CSG_PRIMITIVE_SAMPLER_HPP
#define CIE_CSG_PRIMITIVE_SAMPLER_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/assertions.hpp"
#include "cieutils/packages/concepts/inc/basic_concepts.hpp"
#include "cieutils/packages/concepts/inc/partial_specialization.hpp"

// --- Internal Includes ---
#include "CSG/packages/primitives/inc/Box.hpp"
#include "CSG/packages/primitives/inc/Cube.hpp"
#include "CSG/packages/primitives/inc/Primitive.hpp"

#include "CSG/packages/trees/inc/indexconverter.hpp"
#include "CSG/packages/primitives/inc/csgobject.hpp"


namespace cie::csg {


// ---------------------------------------------------------
// PRIMITIVE SAMPLER INTERFACE
// ---------------------------------------------------------

/// Interface for computing sample points on primitives.
template <concepts::Primitive PrimitiveType>
class PrimitiveSampler :
    public CSGTraits<PrimitiveType::dimension,typename PrimitiveType::coordinate_type>
{
public:
    using primitive_type            = PrimitiveType;

public:
    virtual typename PrimitiveType::point_type getSamplePoint( const PrimitiveType& r_primitive, Size index ) const = 0;
    virtual Size size() const = 0;
};


template <class PrimitiveType>
using PrimitiveSamplerPtr   = std::shared_ptr<PrimitiveSampler<PrimitiveType>>;



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
    const GridIndexConverter<PrimitiveType::dimension>& indexConverter() const;

private:
    GridIndexConverterPtr<PrimitiveType::dimension> _p_indexConverter;
    Size                                            _numberOfPointsPerDimension;
    Size                                            _size;
};


// ---------------------------------------------------------
// SPECIALIZED GRID SAMPLERS
// ---------------------------------------------------------

/// Invalid default template
CIE_DEFINE_INVALID_CLASS_TEMPLATE_TO_SPECIALIZE( CartesianGridSampler )


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

#include "CSG/packages/trees/impl/primitive_sampler_impl.hpp"

#endif