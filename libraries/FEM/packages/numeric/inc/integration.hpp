#ifndef CIE_FEM_INTEGRATION_HPP
#define CIE_FEM_INTEGRATION_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/basic_concepts.hpp"
#include "cieutils/packages/types/inc/types.hpp"

// --- Internal Includes ---
#include "FEM/packages/utilities/inc/kernel.hpp"

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
    static const Size                               dimension = Dimension;
    typedef Kernel<NT>                              kernel_type;
    typedef std::array<NT,dimension>                point_type;
    typedef std::function<NT(const point_type&)>    function_type;

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
    AbsQuadrature() = default;

protected:
    point_container         _integrationPoints;
    weight_container        _weights;
    Size                    _integrationOrder;
};



template <  Size Dimension,
            concepts::NumericType NT >
class GaussLegendreQuadrature : public AbsQuadrature<Dimension,NT>
{
public:
    GaussLegendreQuadrature( Size integrationOrder );
};


template <  Size Dimension,
            concepts::NumericType NT >
class GaussLobattoQuadrature : public AbsQuadrature<Dimension,NT>
{
public:
    GaussLobattoQuadrature( Size integrationOrder );
};







// Helper functions for computing integration points and weights
namespace detail {


template <concepts::NumericType NT>
std::pair
<
    std::vector<NT>,
    std::vector<NT>
>
gaussLegendreNodes( Size integrationOrder );


template <concepts::NumericType NT>
std::pair
<
    std::vector<NT>,
    std::vector<NT>
>
gaussLobattoNodes( Size integrationOrder );


template <  Size Dimension,
            concepts::NumericType NT >
std::pair
<
    std::vector<typename AbsIntegrator<Dimension,NT>::point_type>,
    std::vector<NT>
>
tensorProductQuadratureNodes( const std::pair<std::vector<NT>,std::vector<NT>>& abscissaeAndWeights );

} // namespace detail



} // namespace cie::fem

#include "../impl/integration_impl.hpp"


#endif