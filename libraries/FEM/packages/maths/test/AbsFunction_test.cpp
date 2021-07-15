// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "FEM/packages/maths/inc/AbsFunction.hpp"


namespace cie::fem::maths {


template <class T>
concept NotAbsFunction
= !concepts::AbsFunction<T>;


template <NotAbsFunction T>
constexpr bool isAbsFunction()
{
    return false;
}


template <concepts::AbsFunction>
constexpr bool isAbsFunction()
{
    return true;
}


struct DummyFunction
{
    using value_type      = double;
    using argument_type   = double;
    using derivative_type = DummyFunction;
    using derivative_ptr  = std::shared_ptr<derivative_type>;

    value_type operator()( const argument_type& r_argument ) const { return r_argument; }
    value_type evaluate( const argument_type& r_argument ) const { return r_argument; }
    derivative_ptr derivative() const { return std::make_shared<derivative_type>(); }
};


CIE_TEST_CASE( "AbsFunction", "[maths]" )
{
    CIE_TEST_CASE_INIT( "AbsFunction" )

    using ValueType      = double;
    using ArgumentType   = double;
    using DerivativeType = DummyFunction;

    using TestType       = AbsFunction<ValueType, ArgumentType, DerivativeType>;

    CIE_TEST_CHECK( isAbsFunction<DummyFunction>() );
    CIE_TEST_CHECK( isAbsFunction<TestType> );
}


} // namespace cie::fem::maths