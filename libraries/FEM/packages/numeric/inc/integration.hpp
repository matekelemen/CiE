#ifndef CIE_FEM_INTEGRATION_HPP
#define CIE_FEM_INTEGRATION_HPP

// --- Utility Includes ---
#include <cieutils/concepts.hpp>

// --- Internal Includes ---
#include "../../utilities/inc/kernel.hpp"

// --- STL Includes ---
#include <functional>
#include <vector>
#include <array>


namespace cie::fem {


// Integrator interface
template <  Size Dimension,
            concepts::NumericType NT >
class AbsIntegrator
{
public:
    static const Size                       dimension = Dimension;
    typedef Kernel<NT>                      kernel_type;
    typedef std::array<NT,dimension>        point_type;
    typedef std::function<NT(point_type)>   function_type;

public:
    virtual NT operator()( function_type function ) = 0;
};



// Quadrature base
template <  Size Dimension,
            concepts::NumericType NT >
class AbsQuadrature : public AbsIntegrator<Dimension,NT>
{
public:
    typedef std::vector<typename AbsQuadrature::point_type> point_container;
    typedef std::vector<NT>                                 weight_container;

public:
    AbsQuadrature(  const point_container& integrationPoints,
                    const weight_container& weights );

    virtual NT operator()( typename AbsQuadrature::function_type function ) override;

    const point_container& integrationPoints() const    { return _integrationPoints; }
    const weight_container& weights() const             { return _weights; }

protected:
    point_container         _integrationPoints;
    weight_container        _weights;
};



template <  Size Dimension,
            concepts::NumericType NT >
class GaussLegendreQuadrature : public AbsQuadrature<Dimension,NT>
{
public:
    GaussLegendreQuadrature( Size integrationOrder );

protected:
    Size _integrationOrder;
};







namespace detail {

template <concepts::NumericType NT>
std::pair
<
    std::vector<NT>,
    std::vector<NT>
>
gaussLegendreParameters( Size polynomialOrder );

template <  Size Dimension,
            concepts::NumericType NT >
std::pair
<
    std::vector<typename AbsIntegrator<Dimension,NT>::point_type>,
    std::vector<NT>
>
gaussLegendrePoints( Size polynomialOrder );

} // namespace detail


} // namespace cie::fem

#include "../impl/integration_impl.hpp"


#endif