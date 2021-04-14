#ifndef CIE_FEM_MATHS_GENERIC_OPERATION_IMPL_HPP
#define CIE_FEM_MATHS_GENERIC_OPERATION_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"


namespace cie::fem::maths {


template <class ResultType>
GenericOperation<ResultType>::GenericOperation( typename GenericOperation<ResultType>::operation_type&& r_operation,
                                                typename GenericOperation<ResultType>::derivative_factory&& r_derivativeFactory ) :
    _operator( std::move(r_operation) ),
    _derivativeFactory( std::move(r_derivativeFactory) )
{
}


template <class ResultType>
inline typename GenericOperation<ResultType>::value_type
GenericOperation<ResultType>::operator()( const typename GenericOperation<ResultType>::argument_type& r_argument ) const
{
    CIE_BEGIN_EXCEPTION_TRACING

    return this->_operator(
        r_argument
    );

    CIE_END_EXCEPTION_TRACING
}


template <class ResultType>
inline typename GenericOperation<ResultType>::derivative_ptr
GenericOperation<ResultType>::derivative() const
{
    CIE_BEGIN_EXCEPTION_TRACING

    return this->_derivativeFactory();

    CIE_END_EXCEPTION_TRACING
}


template <class ResultType>
typename GenericOperation<ResultType>::derivative_ptr
GenericOperation<ResultType>::notImplementedDerivativeFactory()
{
    CIE_THROW(
        NotImplementedException,
        "No derivative factory is implemented for this operation!"
    )

    return nullptr;
}

} // namespace cie::fem::maths


#endif