#ifndef CIE_CSG_PRIMITIVE_SAMPLER_HPP
#define CIE_CSG_PRIMITIVE_SAMPLER_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/assertions.hpp"

// --- Internal Includes ---
#include "CSG/packages/primitives/inc/primitives.hpp"
#include "CSG/packages/trees/inc/spacetreeutils.hpp"


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
    virtual typename PrimitiveType::point_type operator()( const PrimitiveType& primitive, Size index ) const = 0;
    virtual constexpr Size size() const 
        { CIE_ASSERT(false, "Pure virtual function") return 0; };
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
    virtual typename CubeSampler<N,M,CoordinateType>::point_type operator()
    ( 
        const typename CubeSampler<N, M, CoordinateType>::primitive_type& primitive,
        Size index 
    ) const override;

    virtual constexpr Size size() const override {return intPow(M,N);}
};


template <  Size N, 
            Size M,
            concepts::NumericType CoordinateType = Double >
class BoxSampler : 
    public detail::AbsPrimitiveSampler<Box<N,CoordinateType>,M>
{
public:
    virtual typename BoxSampler<N, M, CoordinateType>::point_type operator()
    ( 
        const typename BoxSampler<N, M, CoordinateType>::primitive_type& primitive,
        Size index 
    ) const override;

    virtual constexpr Size size() const override {return intPow(M,N);}
};


}

#include "CSG/packages/trees/impl/primitive_sampler_impl.hpp"

#endif