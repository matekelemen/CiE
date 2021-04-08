#ifndef CIE_FEM_MATHS_UNIVARIATE_LAGRANGE_BASIS_POLYNOMIAL_IMPL_HPP
#define CIE_FEM_MATHS_UNIVARIATE_LAGRANGE_BASIS_POLYNOMIAL_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/stl_extension/inc/resize.hpp"
#include "cieutils/packages/maths/inc/NChooseK.hpp"

// --- STL Includes ---
#include <algorithm>
#include <vector>


namespace cie::fem::maths {


template <concepts::NumericType NT>
template <concepts::NumericContainer NodeContainer>
UnivariateLagrangeBasisPolynomial<NT>::UnivariateLagrangeBasisPolynomial( NodeContainer&& r_nodes, Size basisIndex )
{
    CIE_BEGIN_EXCEPTION_TRACING

    const Size polynomialOrder = r_nodes.size();

    utils::resize( this->_coefficients, polynomialOrder );

    // Get rid of the basis node and invert the rest
    const NT basisNode = r_nodes[basisIndex];
    
    std::vector<NT> nodes( polynomialOrder - 1 );
    auto transform = []( NT value ) -> NT { return -value; };

    std::transform( r_nodes.begin(),
                    r_nodes.begin() + basisIndex,
                    nodes.begin(),
                    transform );

    if ( basisIndex < polynomialOrder-1 )
        std::transform( r_nodes.begin() + basisIndex + 1,
                        r_nodes.end(),
                        nodes.begin() + basisIndex,
                        transform );

    // Compute the denominator first
    NT denominator = 1;
    for ( NT node : nodes )
        denominator *= basisNode + node;

    // Backward loop through exponents
    for ( Size numberOfSelectedNodes=0; numberOfSelectedNodes<polynomialOrder; ++numberOfSelectedNodes )
    {
        const Size exponent = polynomialOrder - 1 - numberOfSelectedNodes;

        utils::NChooseK permutation( polynomialOrder - 1, numberOfSelectedNodes );

        NT coefficient = 0;

        do
        {
            NT term = 1;
            auto it_selectedNodeEnd = permutation.end();

            for ( auto it_nodeID=permutation.begin(); it_nodeID!=it_selectedNodeEnd; ++it_nodeID )
                term *= nodes[*it_nodeID];

            coefficient += term;

        } while ( permutation.next() );

        this->_coefficients[exponent] = coefficient / denominator;
    }

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::fem::maths


#endif