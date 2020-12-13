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