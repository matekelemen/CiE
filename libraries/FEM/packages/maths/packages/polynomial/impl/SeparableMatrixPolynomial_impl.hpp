#ifndef CIE_FEM_MATHS_SEPARABLE_MATRIX_POLYNOMIAL_IMPL_HPP
#define CIE_FEM_MATHS_SEPARABLE_MATRIX_POLYNOMIAL_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/stl_extension/inc/resize.hpp"


namespace cie::fem::maths {


template <Size VD0, Size VD1, Size D, concepts::NumericType NT>
SeparableMatrixPolynomial<VD0,VD1,D,NT>::SeparableMatrixPolynomial( typename SeparableMatrixPolynomial<VD0,VD1,D,NT>::polynomial_container&& r_vectorPolynomials ) :
    _vectorPolynomials( std::move(r_vectorPolynomials) )
{
}


template <Size VD0, Size VD1, Size D, concepts::NumericType NT>
SeparableMatrixPolynomial<VD0,VD1,D,NT>::SeparableMatrixPolynomial( const typename SeparableMatrixPolynomial<VD0,VD1,D,NT>::polynomial_container& r_vectorPolynomials ) :
    _vectorPolynomials( r_vectorPolynomials )
{
}


template <Size VD0, Size VD1, Size D, concepts::NumericType NT>
inline typename SeparableMatrixPolynomial<VD0,VD1,D,NT>::value_type
SeparableMatrixPolynomial<VD0,VD1,D,NT>::operator()( const typename SeparableMatrixPolynomial<VD0,VD1,D,NT>::argument_type& r_argument ) const
{
    CIE_BEGIN_EXCEPTION_TRACING

    typename SeparableMatrixPolynomial<VD0,VD1,D,NT>::value_type value;

    auto it_vectorPolynomialEnd = this->_vectorPolynomials.end();
    auto it_value = value.begin();

    for ( auto it_vectorPolynomial=this->_vectorPolynomials.begin(); it_vectorPolynomial!=it_vectorPolynomialEnd; ++it_vectorPolynomial )
    {
        auto vector = it_vectorPolynomial->operator()( r_argument );
        for ( auto component : vector )
            *it_value++ = component;
    }

    return value;

    CIE_END_EXCEPTION_TRACING
}


template <Size VD0, Size VD1, Size D, concepts::NumericType NT>
typename SeparableMatrixPolynomial<VD0,VD1,D,NT>::derivative_type
SeparableMatrixPolynomial<VD0,VD1,D,NT>::derivative() const
{
    return NotImplementedFunction();
}


} // namespace cie::fem::maths


#endif