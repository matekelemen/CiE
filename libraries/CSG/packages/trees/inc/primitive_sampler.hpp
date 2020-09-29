#ifndef CIE_CSG_PRIMITIVE_SAMPLER_HPP
#define CIE_CSG_PRIMITIVE_SAMPLER_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/assertions.hpp"

// --- Internal Includes ---
#include "CSG/packages/primitives/inc/primitives.hpp"
#include "CSG/packages/trees/inc/spacetreeutils.hpp"


namespace cie::csg {


// ---------------------------------------------------------
// PRIMITIVE SAMPLER INTERFACE
// ---------------------------------------------------------

/**
 * Interface for computing sample points on primitives.
*/
template <concepts::PrimitiveType PrimitiveType>
class PrimitiveSampler
{
public:
    static const Size dimension     = PrimitiveType::dimension;
    using primitive_type            = PrimitiveType;

public:
    virtual typename PrimitiveType::point_type getSamplePoint( const PrimitiveType& primitive, Size index ) const = 0;
    virtual Size size() const 
        { CIE_ASSERT(false, "Pure virtual function") return 0; };
};


template <class PrimitiveType>
using PrimitiveSamplerPtr   = std::shared_ptr<PrimitiveType>;



/**
 * Interface for grid-based samplers. 
*/
template <concepts::PrimitiveType PrimitiveType>
class GridSampler : public PrimitiveSampler<PrimitiveType>
{
public:
    virtual Size size() const override;
    Size numberOfPointsPerDimension() const;
    GridSampler<PrimitiveType>& setNumberOfPointsPerDimension( Size numberOfPointsPerDimension );

private:
    Size _numberOfPointsPerDimension;
    Size _size;
};


// ---------------------------------------------------------
// SPECIALIZED PRIMITIVE SAMPLERS
// ---------------------------------------------------------

/**
 * Generate points on a cube grid.
*/
template <  Size Dimension,
            concepts::NumericType CoordinateType = Double >
class CubeSampler : public detail::GridSampler<Cube<Dimension,CoordinateType>>
{
public:
    CubeSampler( Size numberOfPointsPerDimension );

    virtual typename CubeSampler<Dimension,CoordinateType>::point_type getSamplePoint
    (
        const typename CubeSampler<Dimension,CoordinateType>::primitive_type& primitive,
        Size index
    ) const override;
};


/**
 * Generate points on a box grid.
*/
template <  Size Dimension, 
            concepts::NumericType CoordinateType = Double >
class BoxSampler : public detail::GridSampler<Box<Dimension,CoordinateType>>
{
public:
    virtual typename BoxSampler<Dimension,CoordinateType>::point_type getSamplePoint
    ( 
        const typename BoxSampler<Dimension,CoordinateType>::primitive_type& primitive,
        Size index 
    ) const override;
};


} // namespace cie::csg

#include "CSG/packages/trees/impl/primitive_sampler_impl.hpp"

#endif