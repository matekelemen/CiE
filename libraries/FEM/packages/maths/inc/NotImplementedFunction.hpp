#ifndef CIE_FEM_MATHS_NOT_IMPLEMENTED_FUNCTION_HPP
#define CIE_FEM_MATHS_NOT_IMPLEMENTED_FUNCTION_HPP

// --- Internal Includes ---
#include "FEM/packages/maths/inc/AbsFunction.hpp"


namespace cie::fem::maths {


namespace detail {
class AllType
{
public:
    template <class ...Arguments>
    AllType( Arguments&&... r_arguments );

    template <class ...Arguments>
    AllType operator()( Arguments&&... r_arguments ) const;

    template <class ArgumentType>
    AllType operator[]( ArgumentType&& r_argument ) const;

    template <class T>
    operator T ();
}; // class AllType
} // namespace detail


class NotImplementedFunction : public AbsFunction<detail::AllType,detail::AllType,NotImplementedFunction>
{
private:
    using base_type = AbsFunction<detail::AllType,detail::AllType,NotImplementedFunction>;

public:
    base_type::value_type operator()( const base_type::argument_type& r_argument ) const override;

    base_type::derivative_ptr derivative() const override;
}; // class NotImplementedFunction


using NotImplementedFunctionPtr = std::shared_ptr<NotImplementedFunction>;


} // namespace cie::fem::maths

#include "FEM/packages/maths/impl/NotImplementedFunction_impl.hpp"

#endif