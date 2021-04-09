#ifndef CIE_FEM_MATHS_NOT_IMPLEMENTED_FUNCTION_HPP
#define CIE_FEM_MATHS_NOT_IMPLEMENTED_FUNCTION_HPP

// --- Internal Includes ---
#include "FEM/packages/maths/inc/AbsFunction.hpp"


namespace cie::fem::maths {


namespace detail {
struct VoidType {};
} // namespace detail


class NotImplementedFunction : public AbsFunction<detail::VoidType,detail::VoidType,NotImplementedFunction>
{
private:
    using base_type = AbsFunction<detail::VoidType,detail::VoidType,NotImplementedFunction>;

public:
    base_type::value_type operator()( const base_type::argument_type& r_argument ) const override;

    base_type::derivative_ptr derivative() const override;
};


} // namespace cie::fem::maths


#endif