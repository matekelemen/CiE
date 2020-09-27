// --- External Includes ---
#include "catch.hpp"

// --- Internal Includes ---
#include "FEM/packages/physics/inc/abs_element_physics.hpp"
#include "FEM/packages/discretization/inc/abs_element.hpp"

// --- STL Includes ---
#include <array>
#include <algorithm>
#include <iostream>

namespace cie::fem {



TEST_CASE( "AbsElementPhysics", "[physics]" )
{
    const Size Dimension            = 1;
    const Size integrationOrder     = 5;
    using NT                        = Double;
    using Basis                     = LinearBasisFunctionSet<Dimension,NT>;
    using ElementBase               = AbsElement1D<Basis>;
    using Element                   = AbsElementPhysics<ElementBase>;

    auto domain = std::make_pair<NT,NT>( -1.0, 1.0 ); 
    typename Element::dof_container dofs    = { 0, 1 };

    // Check constructor
    REQUIRE_NOTHROW( Element(domain,dofs) );
    Element element(domain,dofs);

    // Check integrator replacement
    REQUIRE_NOTHROW( element.setIntegrator<GaussLegendreQuadrature<Dimension,NT>>(integrationOrder) );
    REQUIRE_NOTHROW( element.integrator() );
    CHECK( element.integrator().integrationPoints().size() == integrationOrder );

    // Check cache
    REQUIRE_NOTHROW( element.basisValues() );
    REQUIRE_NOTHROW( element.basisDerivativeValues() );

    // Check sizes of stored containers
    const auto& basisValues         = element.basisValues();
    const auto& basisDerivatives    = element.basisDerivativeValues();
    REQUIRE( basisValues.size() == integrationOrder );
    REQUIRE( basisDerivatives.size() == integrationOrder );
    for (Size pointIndex=0; pointIndex<integrationOrder; ++pointIndex)
    {
        REQUIRE( basisValues[pointIndex].size() == 2 );
        REQUIRE( basisDerivatives[pointIndex].size() == 2 );
        for (const auto& row : basisDerivatives[pointIndex])
            REQUIRE( row.size() == 1 );
    }

    // Check integration - "mass matrix"
    std::array<std::array<NT,2>,2> matrix;
    for (auto& row : matrix)
        std::fill(  row.begin(),
                    row.end(),
                    0.0 );

    for (Size i=0; i<2; ++i)
        for (Size j=0; j<2; ++j)
        {
            std::vector<NT> integrandValues;
            for (const auto& values : basisValues)
                integrandValues.push_back( values[i]*values[j] );
            matrix[i][j] += element.integrator(integrandValues.begin());
        }

    for (Size i=0; i<2; ++i)
        for (Size j=0; j<2; ++j)
        {
            if (i==j)
                CHECK( matrix[i][j] == Approx(2.0/3.0) );
            else
                CHECK( matrix[i][j] == Approx(1.0/3.0) );
        }
    
    // Check cache clearing
    REQUIRE_NOTHROW( element.clearCache() );
    CHECK_THROWS( element.basisValues() );
    CHECK_THROWS( element.basisDerivativeValues() );
} // TEST_CASE AbsElementPhysics


} // namespace cie::fem