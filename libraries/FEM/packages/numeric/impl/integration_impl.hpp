#ifndef CIE_FEM_INTEGRATION_IMPL_HPP
#define CIE_FEM_INTEGRATION_IMPL_HPP

// --- Utility Includes ---
#include <cieutils/macros.hpp>
#include <cieutils/exceptions.hpp>

// --- STL Includes ---
#include <algorithm>
#include <numeric>


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



template <  Size Dimension,
            concepts::NumericType NT >
GaussLegendreQuadrature<Dimension,NT>::GaussLegendreQuadrature( Size integrationOrder ) {}





/*
namespace detail {

template <concepts::NumericType NT>
std::pair
<
    std::vector<NT>,
    std::vector<NT>
>
gaussLegendreParameters( Size polynomialOrder )
{
    if (polynomialOrder > 5)
        CIE_THROW( NotImplementedException, "Gauss-Legendre integration is only implemented up to order 5!" )

    // TODO
    return std::pair<std::vector<NT>,std::vector<NT>>();
}


template <  Size Dimension,
            concepts::NumericType NT >
std::pair
<
    std::vector<typename AbsIntegrator<Dimension,NT>::point_type>,
    std::vector<NT>
>
gaussLegendrePoints( Size polynomialOrder )
{
    // Init
    std::pair
    <
        std::vector<typename AbsIntegrator<Dimension,NT>::point_type>,
        std::vector<NT>
    > output;
    auto& points                = output.first;
    auto& weights               = output.second;

    auto params                 = gaussLegendreParameters<NT>(polynomialOrder);
    auto& coordinates           = params.first;
    auto& coordinateWeights     = params.second;

    std::array<typename decltype(coordinates)::iterator, Dimension>         coordinateState;
    std::array<typename decltype(coordinateWeights)::iterator,Dimension>    weightState;

    std::fill(  coordinateState.begin(),
                coordinateState.end(),
                coordinates.begin() );

    std::fill(  weightState.begin(),
                weightState.end(),
                coordinateWeights.begin() );
    
    auto cStateIt   = coordinateState.begin();
    auto wStateIt   = weightState.begin();

    // Tensor product of abscissas
    while ( coordinateState.back() != coordinates.end() )
    {
        // Append set of points with the current one
        points.push_back();
        std::transform( coordinateState.begin(),
                        coordinateState.end(),
                        points.back().begin(),
                        [](auto it){return *it;} );

        // Append set of weights with the current one
        weights.push_back(
            std::accumulate(    weightState.begin(),
                                weightState.end(),
                                NT(1.0),
                                [](const auto& rhs, const auto& lhs) -> NT
                                { return (*lhs)*(*rhs); } );
        );

        // Increment state
        if( *cStateIt != coordinates.end() )
        {
            (*cStateIt)++;
            (*wStateIt)++;
        }
        else
        {
            cStateIt++;
            wStateIt++;
        }
    }
}


} // namespace detail
*/


} // namespace cie::fem


#endif