#ifndef CIE_FEM_MATHS_TENSOR_PRODUCT_ANSATZ_SET_IMPL_HPP
#define CIE_FEM_MATHS_TENSOR_PRODUCT_ANSATZ_SET_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/stl_extension/inc/state_iterator.hpp"
#include "cieutils/packages/maths/inc/power.hpp"
#include "cieutils/packages/stl_extension/inc/resize.hpp"
#include "cieutils/packages/macros/inc/exceptions.hpp"


namespace cie::fem::maths {


template <Size Dimension, concepts::NumericType NT>
template <concepts::PointerContainer BasisContainer>
requires concepts::DerivedFrom<typename std::remove_reference<BasisContainer>::type::value_type::element_type,ScalarFunction<Dimension,NT>>
TensorProductAnsatzSet<Dimension,NT>::TensorProductAnsatzSet( BasisContainer&& r_basisFunctions )
{
    CIE_BEGIN_EXCEPTION_TRACING

    const Size numberOfBasisFunctions = r_basisFunctions.size();
    const Size numberOfAnsatzFunctions = intPow( numberOfBasisFunctions, Dimension );

    utils::reserve( this->_ansatzFunctions, numberOfAnsatzFunctions );

    for ( Size ansatzIndex=0; ansatzIndex<numberOfAnsatzFunctions; ++ansatzIndex )
    {
        // TODO
    }

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::fem::maths


#endif