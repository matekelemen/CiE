#ifndef CIE_FEM_MATHS_SEPARABLE_MATRIX_FUNCTION_IMPL_HPP
#define CIE_FEM_MATHS_SEPARABLE_MATRIX_FUNCTION_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/stl_extension/inc/resize.hpp"


namespace cie::fem::maths {


template <Size VD0, Size VD1, Size D, concepts::NumericType NT>
SeparableMatrixFunction<VD0,VD1,D,NT>::SeparableMatrixFunction( typename SeparableMatrixFunction<VD0,VD1,D,NT>::function_container&& r_functions ) :
    _functions( std::move(r_functions) )
{
}


template <Size VD0, Size VD1, Size D, concepts::NumericType NT>
SeparableMatrixFunction<VD0,VD1,D,NT>::SeparableMatrixFunction( const typename SeparableMatrixFunction<VD0,VD1,D,NT>::function_container& r_functions ) :
    _functions( r_functions )
{
}


template <Size VD0, Size VD1, Size D, concepts::NumericType NT>
inline typename SeparableMatrixFunction<VD0,VD1,D,NT>::value_type
SeparableMatrixFunction<VD0,VD1,D,NT>::operator()( const typename SeparableMatrixFunction<VD0,VD1,D,NT>::argument_type& r_argument ) const
{
    CIE_BEGIN_EXCEPTION_TRACING

    typename SeparableMatrixFunction<VD0,VD1,D,NT>::value_type value;

    auto it_functionEnd = this->_functions.end();
    auto it_value = value.begin();

    for ( auto it_function=this->_functions.begin(); it_function!=it_functionEnd; ++it_function )
    {
        auto vector = (*it_function)->operator()( r_argument );
        for ( auto component : vector )
            *it_value++ = component;
    }

    return value;

    CIE_END_EXCEPTION_TRACING
}


template <Size VD0, Size VD1, Size D, concepts::NumericType NT>
typename SeparableMatrixFunction<VD0,VD1,D,NT>::derivative_ptr
SeparableMatrixFunction<VD0,VD1,D,NT>::derivative() const
{
    return typename SeparableMatrixFunction<VD0,VD1,D,NT>::derivative_ptr(
        new NotImplementedFunction
    );
}


} // namespace cie::fem::maths


#endif