#ifndef CIE_FEM_INTEGRATION_IMPL_HPP
#define CIE_FEM_INTEGRATION_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/assertions.hpp"
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/exceptions/inc/exception.hpp"
#include "cieutils/packages/stl_extension/inc/resize.hpp"
#include "cieutils/packages/stl_extension/inc/state_iterator.hpp"
#include "cieutils/packages/maths/inc/power.hpp"

// --- STL Includes ---
#include <algorithm>
#include <numeric>
#include <cmath>
#include <functional>


namespace cie::fem {


template <  Size Dimension,
            concepts::NumericType NT >
AbsQuadrature<Dimension,NT>::AbsQuadrature( const typename AbsQuadrature::point_container& integrationPoints,
                                            const typename AbsQuadrature::weight_container& weights ) :
    _integrationPoints(integrationPoints),
    _weights(weights),
    _integrationOrder(weights.size())
{
    CIE_ASSERT(
        _integrationPoints.size() == weights.size(),
        "Number of points and weights must match"
    );
}


template <  Size Dimension,
            concepts::NumericType NT >
inline NT
AbsQuadrature<Dimension,NT>::operator()( typename AbsQuadrature::function_type function ) const
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
template <class IteratorType>
inline NT
AbsQuadrature<Dimension,NT>::operator()( IteratorType functionValueIt ) const
requires concepts::ClassIterator<IteratorType,NT>
{
    return std::inner_product(  _weights.begin(),
                                _weights.end(),
                                functionValueIt,
                                0.0 );
}



template <  Size Dimension,
            concepts::NumericType NT >
GaussLegendreQuadrature<Dimension,NT>::GaussLegendreQuadrature( Size integrationOrder )
{
    auto pointsAndWeights = detail::tensorProductQuadratureNodes<Dimension,NT>(
        detail::gaussLegendreNodes<NT>(integrationOrder)
    );
    this->_integrationPoints    = pointsAndWeights.first;
    this->_weights              = pointsAndWeights.second;
    this->_integrationOrder     = integrationOrder;

    CIE_ASSERT( 
        this->_integrationPoints.size() == this->_weights.size(),
        "Number of points and weights must match"
    )
}


template <  Size Dimension,
            concepts::NumericType NT >
GaussLobattoQuadrature<Dimension,NT>::GaussLobattoQuadrature( Size integrationOrder )
{
    auto pointsAndWeights = detail::tensorProductQuadratureNodes<Dimension,NT>(
        detail::gaussLobattoNodes<NT>(integrationOrder)
    );
    this->_integrationPoints    = pointsAndWeights.first;
    this->_weights              = pointsAndWeights.second;
    this->_integrationOrder     = integrationOrder;

    CIE_ASSERT( 
        this->_integrationPoints.size() == this->_weights.size(),
        "Number of points and weights must match"
    )
}





namespace detail {

// Temporary solution: hardcoded values
// TODO: implement general solution
//          ( hints: Boost Math Toolkit or GNU Scientific Library )
template <concepts::NumericType NT>
inline std::pair
<
    std::vector<NT>,
    std::vector<NT>
>
gaussLegendreNodes( Size integrationOrder )
{
    std::pair
    <
        std::vector<NT>,
        std::vector<NT>
    > output;
    auto& abscissae = output.first;
    auto& weights   = output.second;

    cie::utils::resize( abscissae, integrationOrder );
    cie::utils::resize( weights, integrationOrder );
    
    switch ( integrationOrder )
    {
        case 1:
            abscissae[0]    = 0.0;
            weights[0]      = 2.0;
            break;
        case 2:
            abscissae[0]    = 5.773502691896258e-01;    weights[0]      = 1.0;
            abscissae[1]    = -5.773502691896258e-01;   weights[1]      = 1.0;
            break;
        case 3:
            abscissae[0]    = 7.745966692414834e-01;    weights[0]      = 5.555555555555556e-01;
            abscissae[1]    = -0.000000000000000e-01;   weights[1]      = 8.888888888888889e-01;
            abscissae[2]    = -7.745966692414834e-01;   weights[2]      = 5.555555555555556e-01;
            break;
        case 4:
            abscissae[0]    = 8.611363115940526e-01;    weights[0]      = 3.478548451374539e-01;
            abscissae[1]    = 3.399810435848563e-01;    weights[1]      = 6.521451548625461e-01;
            abscissae[2]    = -3.399810435848563e-01;   weights[2]      = 6.521451548625461e-01;
            abscissae[3]    = -8.611363115940526e-01;   weights[3]      = 3.478548451374539e-01;
            break;
        case 5:
            abscissae[0]    = 9.061798459386640e-01;    weights[0]      = 2.369268850561891e-01;
            abscissae[1]    = 5.384693101056831e-01;    weights[1]      = 4.786286704993665e-01;
            abscissae[2]    = -0.000000000000000e-01;   weights[2]      = 5.688888888888889e-01;
            abscissae[3]    = -5.384693101056831e-01;   weights[3]      = 4.786286704993665e-01;
            abscissae[4]    = -9.061798459386640e-01;   weights[4]      = 2.369268850561891e-01;
            break;
        default:
            CIE_THROW(  NotImplementedException, 
                        "Gauss-Legendre integration is not implemented for order " + std::to_string(integrationOrder) );
            break;
    }

    return output;
}


// Temporary solution: hardcoded values
// TODO: implement general solution
//       ( hints: Boost Math Toolkit or GNU Scientific Library, 
//         but neither one of them has a readily available implementation.
//         Another possibility is Alglib )
template <concepts::NumericType NT>
inline std::pair
<
    std::vector<NT>,
    std::vector<NT>
>
gaussLobattoNodes( Size integrationOrder )
{
    std::pair
    <
        std::vector<NT>,
        std::vector<NT>
    > output;
    auto& abscissae = output.first;
    auto& weights   = output.second;

    utils::resize( abscissae, integrationOrder );
    utils::resize( weights, integrationOrder );
    
    switch ( integrationOrder )
    {
        case 1:
            abscissae[0]    = 0.0;
            weights[0]      = 2.0;
            break;
        case 2:
            abscissae[0]    = -1.0;                         weights[0]      = 1.0;
            abscissae[1]    = 1.0;                          weights[1]      = 1.0;
            break;
        case 3:
            abscissae[0]    = -1.0;                         weights[0]      = 1.0/3.0;
            abscissae[1]    = 0.0;                          weights[1]      = 4.0/3.0;
            abscissae[2]    = 1.0;                          weights[2]      = 1.0/3.0;
            break;
        case 4:
            abscissae[0]    = -1.0;                         weights[0]      = 1.0/6.0;
            abscissae[1]    = -0.447213595499957939282;     weights[1]      = 5.0/6.0;
            abscissae[2]    = 0.447213595499957939282;      weights[2]      = 5.0/6.0;
            abscissae[3]    = 1.0;                          weights[3]      = 1.0/6.0;
            break;
        case 5:
            abscissae[0]    = -1.0;                         weights[0]      = 0.1;
            abscissae[1]    = -0.6546536707079771437983;    weights[1]      = 0.544444444444444444444;
            abscissae[2]    = 0.0;                          weights[2]      = 0.7111111111111111111111;
            abscissae[3]    = 0.6546536707079771437983;     weights[3]      = 0.544444444444444444444;
            abscissae[4]    = 1.0;                          weights[4]      = 0.1;
            break;
        default:
            CIE_THROW(  NotImplementedException, 
                        "Gauss-Lobatto integration is not implemented for order " + std::to_string(integrationOrder) );
            break;
    }

    return output;
}


template <  Size Dimension,
            concepts::NumericType NT >
std::pair
<
    std::vector<typename AbsIntegrator<Dimension,NT>::point_type>,
    std::vector<NT>
>
tensorProductQuadratureNodes( const std::pair<std::vector<NT>,std::vector<NT>>& abscissaeAndWeights )
{
    // Init
    auto& abscissae             = abscissaeAndWeights.first;
    auto& abscissaWeights       = abscissaeAndWeights.second;
    CIE_ASSERT(
        abscissae.size() == abscissaWeights.size(),
        "Number of abscissae and weights must match!"
    )

    std::pair
    <
        std::vector<typename AbsIntegrator<Dimension,NT>::point_type>,
        std::vector<NT>
    > output;
    auto& points                = output.first;
    auto& weights               = output.second;

    Size numberOfPoints         = intPow( abscissae.size(), Dimension );
    cie::utils::resize( points, numberOfPoints );
    cie::utils::resize( weights, numberOfPoints );

    // Create iterators the loop through all combinations of the abscissae and weights
    auto abscissaState          = cie::utils::makeInternalStateIterator(abscissae, Dimension);
    auto weightState            = cie::utils::makeInternalStateIterator(abscissaWeights, Dimension);
    
    // Compute tensor product components
    for (Size i=0; i<numberOfPoints; ++i)
    {
        cie::utils::resize( points[i], Dimension );
        std::transform( (*abscissaState).begin(),
                        (*abscissaState).end(),
                        points[i].begin(),
                        [](auto it) ->NT 
                            {return *it;} );

        weights[i] = std::accumulate(   (*weightState).begin(),
                                        (*weightState).end(),
                                        NT(1.0),
                                        [](NT lhs, const auto& rhs) -> NT
                                            { return lhs * (*rhs); } );

        ++abscissaState;
        ++weightState;
    }

    return output;
}


} // namespace detail



} // namespace cie::fem


#endif
