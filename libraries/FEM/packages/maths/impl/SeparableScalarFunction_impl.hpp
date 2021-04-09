#ifndef CIE_FEM_MATHS_SEPARABLE_SCALAR_FUNCTION_IMPL_HPP
#define CIE_FEM_MATHS_SEPARABLE_SCALAR_FUNCTION_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/stl_extension/inc/resize.hpp"


namespace cie::fem::maths {


template <Size Dimension, concepts::NumericType NT>
SeparableScalarFunction<Dimension,NT>::SeparableScalarFunction( typename SeparableScalarFunction<Dimension,NT>::univariate_container&& r_univariates ) :
    _univariates( std::move(r_univariates) )
{
}


template <Size Dimension, concepts::NumericType NT>
SeparableScalarFunction<Dimension,NT>::SeparableScalarFunction( const typename SeparableScalarFunction<Dimension,NT>::univariate_container& r_univariates ) :
    _univariates( r_univariates )
{
}


template <Size Dimension, concepts::NumericType NT>
SeparableScalarFunction<Dimension,NT>::SeparableScalarFunction() :
    _univariates()
{
}


template <Size Dimension, concepts::NumericType NT>
inline typename SeparableScalarFunction<Dimension,NT>::value_type
SeparableScalarFunction<Dimension,NT>::operator()( const typename SeparableScalarFunction<Dimension,NT>::argument_type& r_argument ) const
{
    CIE_BEGIN_EXCEPTION_TRACING

    typename SeparableScalarFunction<Dimension,NT>::value_type value = 1;

    auto it_argument = r_argument.begin();

    for ( const auto& rp_univariate : this->_univariates )
        value *= rp_univariate->operator()( *it_argument++ );

    return value;

    CIE_END_EXCEPTION_TRACING
}


template <Size Dimension, concepts::NumericType NT>
typename SeparableScalarFunction<Dimension,NT>::derivative_ptr
SeparableScalarFunction<Dimension,NT>::derivative() const
{
    CIE_BEGIN_EXCEPTION_TRACING

    typename SeparableVectorFunction<Dimension,Dimension,NT>::function_container derivatives;
    utils::resize( derivatives, Dimension );

    auto univariates = this->_univariates;

    for ( Size dim=0; dim<Dimension; ++dim )
    {
        // Reset previous univariate
        if ( 0 < dim )
            univariates[dim-1] = this->_univariates[dim-1];

        // Get derivative for the current variable 
        univariates[dim] = univariates[dim]->derivative();


        // Construct derivative in the current direction
        derivatives[dim] = std::make_shared<SeparableScalarFunction<Dimension,NT>>( univariates );
    }

    return typename SeparableScalarFunction<Dimension,NT>::derivative_ptr(
        new SeparableVectorFunction<Dimension,Dimension,NT>( std::move(derivatives) )
    );

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::fem::maths


#endif