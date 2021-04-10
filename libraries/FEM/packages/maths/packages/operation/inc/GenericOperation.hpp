#ifndef CIE_FEM_MATHS_GENERIC_OPERATION_HPP
#define CIE_FEM_MATHS_GENERIC_OPERATION_HPP

// --- STL Includes ---
#include <functional>


namespace cie::fem::maths {


template <class ResultType>
class GenericOperation : public ResultType
{
public:
    using typename ResultType::value_type;
    using typename ResultType::argument_type;
    using typename ResultType::derivative_ptr;

    using operation_type = std::function<value_type(const argument_type&)>;
    using derivative_factory = std::function<derivative_ptr()>;

public:
    static derivative_ptr notImplementedDerivativeFactory();

    GenericOperation( operation_type&& r_operation,
                      derivative_factory&& r_derivativeFactory = GenericOperation<ResultType>::notImplementedDerivativeFactory );

    virtual value_type operator()( const argument_type& r_argument ) const override;

    virtual derivative_ptr derivative() const override;

protected:
    operation_type     _operator;
    derivative_factory _derivativeFactory;
};


} // namespace cie::fem::maths

#include "FEM/packages/maths/packages/operation/impl/GenericOperation_impl.hpp"

#endif