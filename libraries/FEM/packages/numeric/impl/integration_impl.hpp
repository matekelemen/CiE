#ifndef CIE_FEM_INTEGRATION_IMPL_HPP
#define CIE_FEM_INTEGRATION_IMPL_HPP

// --- Utility Includes ---
#include <cieutils/macros.hpp>
#include <cieutils/exceptions.hpp>


namespace cie::fem {


template <  Size Dimension,
            concepts::NumericType NT >
AbsQuadrature<Dimension,NT>::AbsQuadrature( const point_container& integrationPoints,
                                            const weight_container& weights ) :
    _integrationPoints(integrationPoints),
    _weights(weights)
{
    CIE_ASSERT(
        _integrationPoints.size() == weights.size(),
        "Size of coordinates and weights must match"
    );
}


template <  Size Dimension,
            concepts::NumericType NT >
inline NT
AbsQuadrature<Dimension,NT>::operator()( typename AbsQuadrature::function_type function )
{
    NT value(0.0);
    auto pointIt    = _integrationPoints.begin();
    auto weightIt   = _weights.begin();
    for ( ; pointIt!=_integrationPoints.end(); ++pointIt,++weightIt )
        value += function(*pointIt) * (*weightIt);

    return value;
}







namespace detail {

template <concepts::NumericType NT>
std::vector<NT> gaussLegendreAbscissas( Size polynomialOrder )
{
    if (polynomialOrder > 5)
        CIE_THROW( NotImplementedException, "Gauss-Legendre integration is only implemented up to order 5!" )
}

}


} // namespace cie::fem


#endif