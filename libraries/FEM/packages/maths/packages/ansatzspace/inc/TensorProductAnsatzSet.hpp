#ifndef CIE_FEM_MATHS_TENSOR_PRODUCT_ANSATZ_SET_HPP
#define CIE_FEM_MATHS_TENSOR_PRODUCT_ANSATZ_SET_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/container_concepts.hpp"

// --- Internal Includes ---
#include "FEM/packages/maths/packages/ansatzspace/inc/AnsatzSet.hpp"
#include "FEM/packages/maths/inc/ScalarFunction.hpp"


namespace cie::fem::maths {


template <Size Dimension, concepts::NumericType NT>
class TensorProductAnsatzSet : public AnsatzSet<Dimension,NT>
{
public:
    template <concepts::STLContainer BasisContainer>
    requires concepts::DerivedFrom<typename std::remove_reference<BasisContainer>::type::value_type,ScalarFunction<Dimension,NT>>
    TensorProductAnsatzSet( BasisContainer&& r_basisFunctions );
};


} // namespace cie::fem::maths


#endif