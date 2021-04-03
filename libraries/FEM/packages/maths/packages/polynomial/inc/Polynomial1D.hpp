#ifndef CIE_FEM_MATHS_POLYNOMIAL_1D_HPP
#define CIE_FEM_MATHS_POLYNOMIAL_1D_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/container_concepts.hpp"

// --- Internal Includes ---
#include "FEM/packages/maths/inc/ScalarFunction.hpp"

// --- STL Includes ---
#include <vector>


namespace cie::fem::maths {


template <concepts::NumericType NT>
class Polynomial1D : public ScalarFunction<1,NT,Polynomial1D<NT>>
{
public:
    using coefficient_container = std::vector<NT>;

public:
    Polynomial1D( coefficient_container&& r_coefficients );

    Polynomial1D( const coefficient_container& r_coefficients );

    Polynomial1D();

    Polynomial1D( const Polynomial1D<NT>& r_rhs );

    virtual typename Polynomial1D::value_type operator()( const typename Polynomial1D<NT>::argument_type& r_argument ) const override;

    typename Polynomial1D::value_type operator()( NT argument ) const;

    virtual typename Polynomial1D<NT>::derivative_type derivative() const override;

protected:
    coefficient_container _coefficients;
};


} // namespace cie::fem::maths

#include "FEM/packages/maths/packages/polynomial/impl/Polynomial1D_impl.hpp"

#endif