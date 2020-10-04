#ifndef CIE_CSG_PRIMITIVE_SAMPLER_HPP
#define CIE_CSG_PRIMITIVE_SAMPLER_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/assertions.hpp"
#include "cieutils/packages/concepts/inc/basic_concepts.hpp"

// --- Internal Includes ---
#include "CSG/packages/primitives/inc/primitives.hpp"
#include "CSG/packages/trees/inc/indexconverter.hpp"
#include "CSG/packages/primitives/inc/csgobject.hpp"


namespace cie::csg {


// ---------------------------------------------------------
// PRIMITIVE SAMPLER INTERFACE
// ---------------------------------------------------------

/**
 * Interface for computing sample points on primitives.
*/
template <concepts::PrimitiveType PrimitiveType>
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



/**
 * Interface for grid-based samplers. 
*/
template <concepts::PrimitiveType PrimitiveType>
class GridSampler : public PrimitiveSampler<PrimitiveType>
{
public:
    GridSampler( Size numberOfPointsPerDimension );

    virtual Size size() const override;
    Size numberOfPointsPerDimension() const;
    GridSampler<PrimitiveType>& setNumberOfPointsPerDimension( Size numberOfPointsPerDimension );

protected:
    const GridIndexConverter<PrimitiveType::dimension>& indexConverter() const;

private:
    GridIndexConverterPtr<PrimitiveType::dimension> _p_indexConverter;
    Size                                            _numberOfPointsPerDimension;
    Size                                            _size;
};


// ---------------------------------------------------------
// SPECIALIZED PRIMITIVE SAMPLERS
// ---------------------------------------------------------

/**
 * Generate points on a cube grid.
*/
template <  Size Dimension,
            concepts::NumericType CoordinateType = Double >
class CubeSampler : public GridSampler<Cube<Dimension,CoordinateType>>
{
public:
    CubeSampler( Size numberOfPointsPerDimension );

    virtual typename CubeSampler<Dimension,CoordinateType>::point_type getSamplePoint
    (
        const typename CubeSampler<Dimension,CoordinateType>::primitive_type& r_primitive,
        Size index
    ) const override;
};


/**
 * Generate points on a box grid.
*/
template <  Size Dimension, 
            concepts::NumericType CoordinateType = Double >
class BoxSampler : public GridSampler<Box<Dimension,CoordinateType>>
{
public:
    BoxSampler( Size numberOfPointsPerDimension );

    virtual typename BoxSampler<Dimension,CoordinateType>::point_type getSamplePoint
    ( 
        const typename BoxSampler<Dimension,CoordinateType>::primitive_type& r_primitive,
        Size index 
    ) const override;
};


} // namespace cie::csg

#include "CSG/packages/trees/impl/primitive_sampler_impl.hpp"

#endif