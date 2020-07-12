#ifndef CIE_CSG_PRIMITIVE_SAMPLER_HPP
#define CIE_CSG_PRIMITIVE_SAMPLER_HPP

// --- Internal Includes ---
#include "../../primitives/inc/primitives.hpp"


namespace cie::csg {


// ---------------------------------------------------------
// ABSTRACT PRIMITIVE SAMPLER
// ---------------------------------------------------------
namespace detail {

// Class for computing sample points on primitives
template <class PrimitiveType, Size M>
class AbsPrimitiveSampler
{
public:
    static const Size       dimension = PrimitiveType::dimension;
    static const Size       resolution = M;

    typedef PrimitiveType                               primitive_type;
    typedef typename primitive_type::coordinate_type    coordinate_type;
    typedef typename primitive_type::point_type         point_type;

public:
    virtual typename PrimitiveType::point_type getSamplePoint( const PrimitiveType& primitive, Size index ) const = 0;
};

} // namespace detail


// ---------------------------------------------------------
// SPECIALIZED PRIMITIVE SAMPLERS
// ---------------------------------------------------------

template <  Size N, 
            Size M,
            concepts::NumericType CoordinateType = Double >
class CubeSampler : 
    public detail::AbsPrimitiveSampler<Cube<N,CoordinateType>,M>
{
public:
    virtual typename CubeSampler::point_type getSamplePoint
    ( 
        const typename CubeSampler::primitive_type& primitive, 
        Size index 
    ) const override;
};


template <  Size N, 
            Size M,
            concepts::NumericType CoordinateType = Double >
class BoxSampler : 
    public detail::AbsPrimitiveSampler<Box<N,CoordinateType>,M>
{
public:
    virtual typename BoxSampler::point_type getSamplePoint
    ( 
        const typename BoxSampler::primitive_type& primitive, 
        Size index 
    ) const override;
};


}

#include "../impl/primitive_sampler_impl.hpp"

#endif