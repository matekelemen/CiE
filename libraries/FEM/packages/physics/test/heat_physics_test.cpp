// --- External Includes ---
#include "catch.hpp"

// --- Utility Includes ---
#include <cieutils/file.hpp>
#include "cieutils/packages/macros/inc/testing.hpp"

// --- Internal Includes ---
#include "FEM/packages/physics/inc/heat_physics.hpp"
#include "FEM/packages/discretization/inc/abs_element.hpp"

// --- STL Includes ---
#include <algorithm>
#include <array>
#include <iostream>


namespace cie::fem {


TEST_CASE( "StaticLinearHeatPhysics1D", "[physics]" )
{
    CIE_TEST_CASE_INIT( "StaticLinearHeatPhysics1D" )

    using NT                    = Double;
    using BasisSet             = LinearBasisFunctionSet<1,NT>;
    using ElementBase           = AbsElement1D<BasisSet>;
    using Element               = StaticLinearHeatPhysics1D<ElementBase>;

    using Row                   = std::array<NT,2>;
    using Matrix                = std::array<Row,2>;

    // Define element parameters
    auto materialParameter                  = [](NT localCoordinate){ return 2.0; };
    auto load                               = [](NT localCoordinate){ return 1.0; };
    typename Element::dof_container dofs    = { 0, 1 };
    std::pair<NT,NT> domain                 = { 0.0, 1.0 };
    const Size integrationOrder             = 5;

    // Element constructor
    REQUIRE_NOTHROW( Element(materialParameter, load, domain, dofs) );
    Element element(    materialParameter,
                        load,
                        domain,
                        dofs );
    REQUIRE_NOTHROW( element.setIntegrator<GaussLegendreQuadrature<1,NT>>(integrationOrder) );

    // Define global matrix
    Matrix stiffness;
    for (auto& row : stiffness)
        std::fill(  row.begin(),
                    row.end(),
                    0.0 );

    auto updateStiffness = [&stiffness]( Size i, Size j, NT value ) -> void
    {
        stiffness[i][j] += value;
    };

    // Integration
    REQUIRE_NOTHROW( element.integrateStiffness(updateStiffness) );
    CHECK( stiffness[0][0] == Approx(materialParameter(0.0)) );
    CHECK( stiffness[0][1] == Approx(-materialParameter(0.0)) );
    CHECK( stiffness[1][0] == Approx(-materialParameter(0.0)) );
    CHECK( stiffness[1][1] == Approx(materialParameter(0.0)) );

} // TEST_CASE StaticLinearHeatPhysics1D


} // namespace cie::fem