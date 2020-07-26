// --- External Includes ---
#include "catch.hpp"

// --- Utility Includes ---
#include <cieutils/concepts.hpp>

// --- Internal Includes ---
#include "../inc/basisfunctions.hpp"

// --- STL Includes ---
#include <vector>
#include <array>
#include <iostream>


namespace cie::fem
{


TEST_CASE( "LinearBasisFunctionSet", "numeric" )
{
    const Size dim = 2;

    typedef PolynomialBasisFunctionSet<dim,Double> Basis;

    typename Basis::coefficient_container coefficients;
    utils::setContainerSize(coefficients, dim);

    coefficients[0] =
        typename Basis::polynomial_set({
            typename Basis::polynomial_coefficients( {1.0, 1.0, 1.0} ),
            typename Basis::polynomial_coefficients( {0.0, 1.0, 2.0} ),
        });
    coefficients[1] = 
        typename Basis::polynomial_set({
            typename Basis::polynomial_coefficients( {2.0, 1.0} ),
            typename Basis::polynomial_coefficients( {1.0, 0.0, 1.0} ),
            typename Basis::polynomial_coefficients( {1.0} )
        });

    REQUIRE_NOTHROW( Basis(coefficients) );
    Basis basis(coefficients);

    // Check domain
    REQUIRE( basis.domain().size() == dim );
    for (auto& pair : basis.domain())
    {
        CHECK( pair.first   == Approx(-1.0) );
        CHECK( pair.second  == Approx(1.0) );
    }

    // Check polynomial degrees
    CHECK( basis.polynomialDegree(0,0) == 2 );
    CHECK( basis.polynomialDegree(0,1) == 2 );
    CHECK( basis.polynomialDegree(1,0) == 1 );
    CHECK( basis.polynomialDegree(1,1) == 2 );
    CHECK( basis.polynomialDegree(1,2) == 0 );

    // Define samples
    std::vector<Double> sampleVector = { -2.0, -1.0, -0.5, 0.0, 0.5, 1.0, 2.0 };
    std::array<Double,7> sampleArray = { -2.0, -1.0, -0.5, 0.0, 0.5, 1.0, 2.0 };
    std::vector<std::vector<Approx>> referenceValues = 
    {
        std::initializer_list<Approx>( { 0.0,   1.0,    0.75,   1.0,    1.75,   3.0,    0.0 } ),
        std::initializer_list<Approx>( { 0.0,   1.0,    0.0,    0.0,    1.0,    3.0,    0.0 } ),
        std::initializer_list<Approx>( { 0.0,   1.0,    1.5,    2.0,    2.5,    3.0,    0.0 } ),
        std::initializer_list<Approx>( { 0.0,   2.0,    1.25,   1.0,    1.25,   2.0,    0.0 } ),
        std::initializer_list<Approx>( { 0.0,   1.0,    1.0,    1.0,    1.0,    1.0,    0.0 } )
    };

    // Check values - single calls
    REQUIRE( basis.functions().size() == 2 );
    auto functionIt     = basis.functions().begin();
    Size functionIndex  = 0;
    for ( auto& functions : basis.functions() )
    {
        Size dimensionIndex = std::distance(basis.functions().begin(), functionIt);
        REQUIRE( functions.size() == functionIt++->size() );
        for (Size i=0; i<functions.size(); ++i,++functionIndex)
            for (Size sampleIndex = 0; sampleIndex<sampleVector.size(); ++sampleIndex)
            {
                auto& reference = referenceValues[functionIndex][sampleIndex];
                CHECK( functions[i](sampleVector[sampleIndex]) == reference.margin(1e-16) );
                CHECK( basis( dimensionIndex, i, sampleVector[sampleIndex] ) == reference.margin(1e-16) );
            }
    }

    // Helper functions for checking
    // the results of batch calls
    auto checkValueVector = [&referenceValues]( Size functionIndex, const std::vector<Double>& vector )
    {
        REQUIRE( vector.size() == referenceValues[functionIndex].size() );
        auto refIt  = referenceValues[functionIndex].begin();
        auto valIt  = vector.begin();
        for ( ; valIt!=vector.end(); ++refIt,++valIt )
            CHECK( *valIt == *refIt );
    };

    auto checkValueArray = [&referenceValues]( Size functionIndex, const decltype(sampleArray)& vector )
    {
        REQUIRE( vector.size() == referenceValues[functionIndex].size() );
        auto refIt  = referenceValues[functionIndex].begin();
        auto valIt  = vector.begin();
        for ( ; valIt!=vector.end(); ++refIt,++valIt )
            CHECK( *valIt == *refIt );
    };

    // Check values - batch call
    decltype(sampleVector)  valueVector;
    decltype(sampleArray)   valueArray;
    functionIndex = 0;
    for (Size dimension=0; dimension<dim; ++dimension)
        for (Size i=0; i<basis.functions(dimension).size(); ++i)
        {
            valueVector = basis(dimension,i,sampleVector);
            checkValueVector(functionIndex, valueVector);

            valueArray  = basis(dimension,i,sampleArray);
            checkValueArray(functionIndex, valueArray);

            valueVector.clear();
            basis(  dimension, i,
                    sampleVector,
                    valueVector );
            checkValueVector(functionIndex, valueVector);

            valueVector.clear();
            basis(  dimension, i,
                    sampleVector.begin(),
                    sampleVector.end(),
                    std::back_inserter(valueVector) );
            checkValueVector(functionIndex, valueVector);

            functionIndex++;
        }

}


}