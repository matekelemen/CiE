// Create a main() if CIE_DEFINE_TESTRUNNER_MAIN is defined
#ifdef CIE_DEFINE_TESTRUNNER_MAIN
#define CATCH_CONFIG_MAIN
#endif

// --- External Includes ---
#include "catch.hpp" // <-- currently using catch2 for unit and system tests

// --- Internal Includes ---
#include "cieutils/packages/macros/inc/testing.hpp"


#define CIE_TEST_CASE( testCaseName, testCaseTag ) TEST_CASE( testCaseName, testCaseTag )

#define CIE_TEST_CASE_METHOD( testCaseClass, testCaseName, testCaseTag ) TEST_CASE_METHOD( testCaseClass, testCaseName, testCaseTag )

#define CIE_TEST_SECTION( sectionName ) SECTION( sectionName )


// Test singles

#define CIE_EXPAND_ARGUMENTS( arguments ) arguments     // <-- workaround for the buggy MSVC preprocessor...

#define CIE_TEST_CHECK( ... ) CIE_EXPAND_ARGUMENTS( CHECK( __VA_ARGS__ ) )

#define CIE_TEST_CHECK_NOTHROW( ... ) CIE_EXPAND_ARGUMENTS( CHECK_NOTHROW( __VA_ARGS__ ) )

#define CIE_TEST_CHECK_THROWS( ... ) CIE_EXPAND_ARGUMENTS( CHECK_THROWS( __VA_ARGS__ ) )

#define CIE_TEST_CHECK_THROWS_AS( ... ) CIE_EXPAND_ARGUMENTS( CHECK_THROWS_AS( __VA_ARGS__ ) )

#define CIE_TEST_REQUIRE( ... ) CIE_EXPAND_ARGUMENTS( REQUIRE( __VA_ARGS__ ) )

#define CIE_TEST_REQUIRE_NOTHROW( ... ) CIE_EXPAND_ARGUMENTS( REQUIRE_NOTHROW( __VA_ARGS__ ) )

#define CIE_TEST_REQUIRE_THROWS( ... ) CIE_EXPAND_ARGUMENTS( REQUIRE_THROWS( __VA_ARGS__ ) )

#define CIE_TEST_REQUIRE_THROWS_AS( ... ) CIE_EXPAND_ARGUMENTS( REQUIRE_THROWS_AS( __VA_ARGS__ ) )