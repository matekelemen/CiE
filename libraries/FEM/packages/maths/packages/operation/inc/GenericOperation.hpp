#ifndef CIE_FEM_MATHS_GENERIC_OPERATION_HPP
#define CIE_FEM_MATHS_GENERIC_OPERATION_HPP

// --- Internal Includes ---
#include "FEM/packages/maths/packages/operation/inc/Operation.hpp"

// --- STL Includes ---
#include <functional>


namespace cie::fem::maths {


template <class ResultType>
class GenericOperation : public Operation<ResultType>
{
public:
    using typename ResultType::value_type;
    using typename ResultType::argument_type;
    using typename ResultType::derivative_ptr;

    using operation_type = std::function<value_type(const argument_type&)>;
    using derivative_factory = std::function<derivative_ptr()>;

    CIE_DEFINE_CLASS_POINTERS( GenericOperation )

public:
    GenericOperation( operation_type&& r_operation,
                      derivative_factory&& r_derivativeFactory = GenericOperation<ResultType>::notImplementedDerivativeFactory );

    virtual value_type operator()( const argument_type& r_argument ) const override;

    virtual derivative_ptr derivative() const override;

protected:
    static derivative_ptr notImplementedDerivativeFactory();

protected:
    operation_type     _operator;
    derivative_factory _derivativeFactory;
};


} // namespace cie::fem::maths

#include "FEM/packages/maths/packages/operation/impl/GenericOperation_impl.hpp"

#endif