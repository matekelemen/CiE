// --- External Includes ---
#include "catch.hpp"

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/testing.hpp"

// --- Internal Includes ---
#include "cieutils/packages/commandline/inc/CommandLineArguments.hpp"


namespace cie::utils {


struct TestType {};


TEST_CASE( "CommandLineArguments", "[commandline]" )
{
    CIE_TEST_CASE_INIT( "CommandLineArguments" )

    const Size argc = 4;

    {
        CIE_TEST_CASE_INIT( "correct arguments" )

        char const* argv[argc] = {
            "https://github.com",
            "--name=best_name",
            "--float=3.14159265",
            "--integer=2"
        };

        CHECK_NOTHROW( CommandLineArguments(argc, argv) );
    }

    {
        CIE_TEST_CASE_INIT( "no key specified" )

        char const* argv[argc] = {
            "https://github.com",
            "--name=best_name",
            "--=3.14159265",
            "--integer=2"
        };

        CHECK_THROWS( CommandLineArguments(argc, argv) );
    }

    {
        CIE_TEST_CASE_INIT( "duplicate keys" )

        char const* argv[argc] = {
            "https://github.com",
            "--name=best_name",
            "--integer=3.14159265",
            "--integer=2"
        };

        CHECK_THROWS( CommandLineArguments(argc, argv) );
    }

    {
        CIE_TEST_CASE_INIT( "no value specified" )

        char const* argv[argc] = {
            "https://github.com",
            "--name=best_name",
            "--float=",
            "--integer=2"
        };

        CHECK_THROWS( CommandLineArguments(argc, argv) );
    }

    {
        CIE_TEST_CASE_INIT( "no separator" )

        char const* argv[argc] = {
            "https://github.com",
            "--name=best_name",
            "--float3.14159265",
            "--integer=2"
        };

        CHECK_THROWS( CommandLineArguments(argc, argv) );
    }

    {
        CIE_TEST_CASE_INIT( "argument parsing" )

        char const* argv[argc] = {
            "--name=best_name",
            "https://github.com",
            "--float=-3.14159265",
            "--integer=2"
        };

        REQUIRE_NOTHROW( CommandLineArguments(argc, argv) );
        CommandLineArguments args( argc, argv );

        CHECK_NOTHROW( args.get<std::string>(0) );
        CHECK( args.get<std::string>(0) == "https://github.com" );

        CHECK_NOTHROW( args.get<std::string>("name") );
        CHECK( args.get<std::string>("name") == "best_name" );

        CHECK_NOTHROW( args.get<std::string>("float") );
        CHECK( args.get<std::string>("float") == "-3.14159265" );

        CHECK_NOTHROW( args.get<int>("float") );
        CHECK( args.get<int>("float") == -3 );

        CHECK_THROWS( args.get<Size>("float") );

        CHECK_NOTHROW( args.get<float>("float") );
        CHECK( args.get<float>("float") == float(-3.14159265) );

        CHECK_NOTHROW( args.get<double>("float") );
        CHECK( args.get<double>("float") == double(-3.14159265) );

        CHECK_NOTHROW( args.get<std::string>("integer") );
        CHECK( args.get<std::string>("integer") == "2" );

        CHECK_NOTHROW( args.get<int>("integer") );
        CHECK( args.get<int>("integer") == 2 );

        CHECK_NOTHROW( args.get<Size>("integer") );
        CHECK( args.get<Size>("integer") == 2 );

        CHECK_THROWS( args.get<TestType>(0) );
        CHECK_THROWS( args.get<TestType>("") );
    }
}


} // namespace cie::utils