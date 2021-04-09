#ifndef CIE_FEM_MATHS_ABS_FUNCTION_HPP
#define CIE_FEM_MATHS_ABS_FUNCTION_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/basic_concepts.hpp"

// --- STL Includes ---
#include <memory>


namespace cie::fem::maths {


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
    virtual value_type operator()( const ArgumentType& r_argument ) const = 0;
    virtual derivative_ptr derivative() const = 0;
};


} // namespace cie::fem::maths




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
            typename T::derivative_type
        >,
        void
    >
> : public std::true_type {};
}


template <class T>
concept AbsFunction
= detail::IsAbsFunction<T>::value;


} // namespace cie::concepts

#endif