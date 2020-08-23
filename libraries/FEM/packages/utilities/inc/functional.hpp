#ifndef CIE_FEM_UTILITIES_FUNCTIONAL_HPP
#define CIE_FEM_UTILITIES_FUNCTIONAL_HPP

// --- Utility Includes ---
#include <cieutils/concepts.hpp>

// --- STL Includes ---
#include <functional>
#include <array>


namespace cie::fem::utils {


template <  class ReturnType,
            class ArgumentType >
class FunctionWrapper
{
public:
    using return_type       = ReturnType;
    using argument_type     = ArgumentType;
    using function_type     = std::function<ReturnType(const ArgumentType&)>;

public:
    FunctionWrapper( function_type function ) :
        _function(function)
    {}

    return_type operator()( const ArgumentType& arg )   { return _function(arg); }
    function_type get()                                 { return function_type(_function); }

protected:
    function_type _function;
};


template <concepts::NumericType NT>
using ScalarMap     = FunctionWrapper<NT,NT>;

template <  concepts::NumericType NT,
            Size Dimension >
using ScalarFunction        = FunctionWrapper<NT,std::array<NT,Dimension>>;

template <  concepts::NumericType NT,
            Size SourceDimension,
            Size TargetDimension >
using VectorFunction        = FunctionWrapper<std::array<NT,TargetDimension>,std::array<NT,SourceDimension>>;

template <  concepts::NumericType NT,
            Size Dimension >
using VectorMap             = VectorFunction<NT,Dimension,Dimension>;


} // namespace cie::fem::utils


#endif