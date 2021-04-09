#ifndef CIE_FEM_MATHS_UNIVARIATE_SCALAR_FUNCTION_IMPL_HPP
#define CIE_FEM_MATHS_UNIVARIATE_SCALAR_FUNCTION_IMPL_HPP


namespace cie::fem::maths {


template <concepts::NumericType NT>
inline typename UnivariateScalarFunction<NT>::value_type
UnivariateScalarFunction<NT>::operator()( const typename UnivariateScalarFunction<NT>::argument_type& r_argument ) const
{
    return this->operator()( r_argument[0] );
}


} // namespace cie::fem::maths


#endif