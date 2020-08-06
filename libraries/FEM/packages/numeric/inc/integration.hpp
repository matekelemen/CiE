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







namespace detail {

template <concepts::NumericType NT>
std::vector<NT> gaussLegendreAbscissas( Size polynomialOrder );

} // nnamespace detail


} // namespace cie::fem

#include "../impl/integration_impl.hpp"


#endif