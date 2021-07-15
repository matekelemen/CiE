#ifndef CIE_FEM_MATHS_UNIVARIATE_SCALAR_FUNCTION_HPP
#define CIE_FEM_MATHS_UNIVARIATE_SCALAR_FUNCTION_HPP

// --- Internal Includes ---
#include "FEM/packages/maths/inc/ScalarFunction.hpp"


namespace cie::fem::maths {


template <concepts::NumericType NT>
class UnivariateScalarFunction : public ScalarFunction<1,NT,UnivariateScalarFunction<NT>>
{
private:
    using base_type = ScalarFunction<1,NT,UnivariateScalarFunction<NT>>;

public:
    using typename base_type::value_type;
    using typename base_type::argument_type;

public:
    value_type operator()( const argument_type& r_argument ) const override final;

    virtual value_type operator()( NT r_argument ) const = 0;
};


} // namespace cie::fen::maths


namespace cie::concepts {

template <class T, class NT>
concept UnivariateScalarFunction
= DerivedFrom<typename std::decay<T>::type, fem::maths::UnivariateScalarFunction<NT>>;

template <class T, class NT>
concept UnivariateScalarFunctionPtr
= UnivariateScalarFunction<typename std::decay<T>::type::element_type, NT>;

} // namespace cie::concepts


#include "FEM/packages/maths/impl/UnivariateScalarFunction_impl.hpp"

#endif