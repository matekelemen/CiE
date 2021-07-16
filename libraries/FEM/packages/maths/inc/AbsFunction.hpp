#ifndef CIE_FEM_MATHS_ABS_FUNCTION_HPP
#define CIE_FEM_MATHS_ABS_FUNCTION_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/basic_concepts.hpp"

// --- STL Includes ---
#include <memory>


namespace cie::concepts {


namespace detail {
template <typename T, typename _ = void>
struct IsAbsFunction : std::false_type {};

template <typename ...T>
struct IsAbsFunctionHelper {};

template <typename T>
struct IsAbsFunction<
    T,
    std::conditional_t<
        false,
        IsAbsFunctionHelper<
            typename T::value_type,
            typename T::argument_type,
            typename T::derivative_type,
            typename T::derivative_ptr
        >,
        void
    >
> : public std::true_type {};
}


/** Concept of a generic function with:
 *
 *  Type definitions:
 *      value_type
 *      argument_type
 *      derivative_type
 *      derivative_ptr
 *
 *   Member functions:
 *      value_type operator()( argument_type )
 *      value_type evaluate( argument_type )
 *      derivative_ptr derivative()
 */
template <class T>
concept AbsFunction
=  detail::IsAbsFunction<T>::value
&& requires ( T instance )
{
    { instance( typename T::argument_type() ) } -> std::same_as<typename T::value_type>;
    { instance.evaluate( typename T::argument_type() ) } -> std::same_as<typename T::value_type>;
    { instance.derivative() } -> std::same_as<typename T::derivative_ptr>;
};

template <class T>
concept AbsFunctionPtr
=  Pointer<typename std::decay<T>::type>
&& AbsFunction<typename std::decay<T>::type::element_type>;


} // namespace cie::concepts


namespace cie::fem::maths {


/** Abstract mathematical function class defining its argument, value, and derivative
 *  @details this class is intended to be the base of every mathematical function y = f(x),
 *           providing relevant traits as well as an interface for evaluation.
 * 
 *  @param ValueType output type (y)
 *  @param ArgumentType input type (x)
 *  @param DerivativeType type of the function's derivative (must be an AbsFunction as well)
 */
template < class ValueType,
           class ArgumentType,
           class DerivativeType >
class AbsFunction
{
public:
    using value_type        = ValueType;
    using argument_type     = ArgumentType;
    using derivative_type   = DerivativeType;
    using derivative_ptr    = std::shared_ptr<derivative_type>;

public:
    virtual ~AbsFunction() {}

    /// Evaluate the function with the specified argument
    virtual value_type operator()( const ArgumentType& r_argument ) const = 0;

    /// Construct the function's derivative
    virtual derivative_ptr derivative() const = 0;

    /// Convenience function for calling operator() on pointers to AbsFunction
    value_type evaluate( const ArgumentType& r_argument ) const
    { return this->operator()(r_argument); }
};


} // namespace cie::fem::maths


#endif