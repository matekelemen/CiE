#ifndef CIE_FEM_MATHS_TENSOR_PRODUCT_ANSATZ_SET_IMPL_HPP
#define CIE_FEM_MATHS_TENSOR_PRODUCT_ANSATZ_SET_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/stl_extension/inc/state_iterator.hpp"
#include "cieutils/packages/maths/inc/power.hpp"
#include "cieutils/packages/stl_extension/inc/resize.hpp"
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- Internal Includes ---
#include "FEM/packages/maths/inc/SeparableScalarFunction.hpp"

// --- STL Includes ---
#include <numeric>


namespace cie::fem::maths {


template <Size Dimension, concepts::NumericType NT>
template <concepts::PointerContainer BasisContainer>
requires concepts::DerivedFrom<typename std::remove_reference<BasisContainer>::type::value_type::element_type,UnivariateScalarFunction<NT>>
TensorProductAnsatzSet<Dimension,NT>::TensorProductAnsatzSet( BasisContainer&& r_basisFunctions )
{
    CIE_BEGIN_EXCEPTION_TRACING

    const Size numberOfBasisFunctions = r_basisFunctions.size();
    const Size numberOfAnsatzFunctions = intPow( numberOfBasisFunctions, Dimension );

    utils::reserve( this->_ansatzFunctions, numberOfAnsatzFunctions );

    typename SeparableScalarFunction<Dimension,NT>::univariate_container basisProduct;
    utils::resize( basisProduct, Dimension );

    std::vector<Size> states;
    utils::resize( states, numberOfBasisFunctions );
    std::iota( states.begin(), states.end(), 0 );

    auto permutation = utils::makeInternalStateIterator(
        states,
        Dimension
    );

    for ( Size ansatzIndex=0; ansatzIndex<numberOfAnsatzFunctions; ++ansatzIndex,++permutation )
    {
        const auto& r_state = *permutation;

        for ( Size dim=0; dim<Dimension; ++dim )
            basisProduct[dim] = r_basisFunctions[*r_state[dim]];

        this->_ansatzFunctions.push_back(
            typename TensorProductAnsatzSet<Dimension,NT>::ansatz_ptr(
                new SeparableScalarFunction<Dimension,NT>( basisProduct )
            )
        );
    }

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::fem::maths


#endif