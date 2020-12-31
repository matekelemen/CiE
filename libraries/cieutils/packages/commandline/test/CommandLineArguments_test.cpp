// --- Internal Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"
#include "cieutils/packages/commandline/inc/CommandLineArguments.hpp"


namespace cie::utils {


struct TestType {};


CIE_TEST_CASE( "CommandLineArguments", "[commandline]" )
{
    CIE_TEST_CASE_INIT( "CommandLineArguments" )

    const Size argc = 5;

    {
        CIE_TEST_CASE_INIT( "correct arguments" )

        char const* argv[argc] = {
            "./",
            "https://github.com",
            "--name=best_name",
            "--float=3.14159265",
            "--integer=2"
        };

        CIE_TEST_CHECK_NOTHROW( CommandLineArguments(argc, argv) );
    }

    {
        CIE_TEST_CASE_INIT( "no key specified" )

        char const* argv[argc] = {
            "./",
            "https://github.com",
            "--name=best_name",
            "--=3.14159265",
            "--integer=2"
        };

        CIE_TEST_CHECK_THROWS( CommandLineArguments(argc, argv) );
    }

    {
        CIE_TEST_CASE_INIT( "duplicate keys" )

        char const* argv[argc] = {
            "./",
            "https://github.com",
            "--name=best_name",
            "--integer=3.14159265",
            "--integer=2"
        };

        CIE_TEST_CHECK_THROWS( CommandLineArguments(argc, argv) );
    }

    {
        CIE_TEST_CASE_INIT( "no value specified" )

        char const* argv[argc] = {
            "./",
            "https://github.com",
            "--name=best_name",
            "--float=",
            "--integer=2"
        };

        CIE_TEST_CHECK_THROWS( CommandLineArguments(argc, argv) );
    }

    {
        CIE_TEST_CASE_INIT( "no separator" )

        char const* argv[argc] = {
            "./",
            "https://github.com",
            "--name=best_name",
            "--float3.14159265",
            "--integer=2"
        };

        CIE_TEST_CHECK_THROWS( CommandLineArguments(argc, argv) );
    }

    {
        CIE_TEST_CASE_INIT( "argument parsing" )

        char const* argv[argc] = {
            "./",
            "--name=best_name",
            "https://github.com",
            "--float=-3.14159265",
            "--integer=2"
        };

        CIE_TEST_REQUIRE_NOTHROW( CommandLineArguments(argc, argv) );
        CommandLineArguments args( argc, argv );

        CIE_TEST_CHECK_NOTHROW( args.get<std::string>(0) );
        CIE_TEST_CHECK( args.get<std::string>(0) == "https://github.com" );

        CIE_TEST_CHECK_NOTHROW( args.get<std::string>("name") );
        CIE_TEST_CHECK( args.get<std::string>("name") == "best_name" );

        CIE_TEST_CHECK_NOTHROW( args.get<std::string>("float") );
        CIE_TEST_CHECK( args.get<std::string>("float") == "-3.14159265" );

        CIE_TEST_CHECK_NOTHROW( args.get<int>("float") );
        CIE_TEST_CHECK( args.get<int>("float") == -3 );

        CIE_TEST_CHECK_THROWS( args.get<Size>("float") );

        CIE_TEST_CHECK_NOTHROW( args.get<float>("float") );
        CIE_TEST_CHECK( args.get<float>("float") == float(-3.14159265) );

        CIE_TEST_CHECK_NOTHROW( args.get<double>("float") );
        CIE_TEST_CHECK( args.get<double>("float") == double(-3.14159265) );

        CIE_TEST_CHECK_NOTHROW( args.get<std::string>("integer") );
        CIE_TEST_CHECK( args.get<std::string>("integer") == "2" );

        CIE_TEST_CHECK_NOTHROW( args.get<int>("integer") );
        CIE_TEST_CHECK( args.get<int>("integer") == 2 );

        CIE_TEST_CHECK_NOTHROW( args.get<Size>("integer") );
        CIE_TEST_CHECK( args.get<Size>("integer") == 2 );

        CIE_TEST_CHECK_THROWS( args.get<TestType>(0) );
        CIE_TEST_CHECK_THROWS( args.get<TestType>("") );
    }
}


} // namespace cie::utils