#ifndef CIE_FEM_MATHS_TENSOR_PRODUCT_ANSATZ_SET_HPP
#define CIE_FEM_MATHS_TENSOR_PRODUCT_ANSATZ_SET_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/container_concepts.hpp"

// --- Internal Includes ---
#include "FEM/packages/maths/packages/ansatzspace/inc/AnsatzSet.hpp"
#include "FEM/packages/maths/inc/UnivariateScalarFunction.hpp"


namespace cie::fem::maths {


template <Size Dimension, concepts::NumericType NT>
class TensorProductAnsatzSet : public AnsatzSet<Dimension,NT>
{
public:
    TensorProductAnsatzSet() = default;

    template <concepts::PointerContainer BasisContainer>
    requires concepts::DerivedFrom<typename std::remove_reference<BasisContainer>::type::value_type::element_type,UnivariateScalarFunction<NT>>
    TensorProductAnsatzSet( BasisContainer&& r_basisFunctions );
};


} // namespace cie::fem::maths

#include "FEM/packages/maths/packages/ansatzspace/impl/TensorProductAnsatzSet_impl.hpp"

#endif