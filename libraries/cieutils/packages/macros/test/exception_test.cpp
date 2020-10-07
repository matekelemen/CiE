// --- External Includes ---
#include "catch.hpp"

// --- Internal Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/macros/inc/testing.hpp"


namespace cie {


namespace exception {

void test_inner() 
{
    CIE_BEGIN_EXCEPTION_TRACING
    CIE_THROW( DivisionByZeroException, "test" )
    CIE_END_EXCEPTION_TRACING
}

void testFunction()
{
    CIE_BEGIN_EXCEPTION_TRACING
    test_inner();
    CIE_END_EXCEPTION_TRACING
}

} // namespace exception


TEST_CASE( "Exception tracing", "[macros]" )
{
    CIE_TEST_CASE_INIT( "Exception tracing" )

    CHECK_THROWS( exception::testFunction() );

    try
    {
        CIE_BEGIN_EXCEPTION_TRACING
        exception::testFunction();
        CIE_END_EXCEPTION_TRACING
    }
    catch ( const std::exception& r_exception )
    {
        std::cout << r_exception.what() << std::endl;
    }
    
}


} // namespace cie