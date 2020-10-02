// --- External Includes ---
#include "catch.hpp"

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/testing.hpp"

// --- Internal Includes ---
#include "cieutils/packages/stl_extension/inc/type_safety.hpp"

// --- STL Includes ---
#include <vector>


namespace cie::utils {


// Define overloaded functions to check types
namespace test {

using Type0         = std::vector<int>;
STRONG_TYPEDEF( Type0, Type1 )
STRONG_TYPEDEF( Type0, Type2 )

struct TestClass
{
    STRONG_TYPEDEF( Type0, MemberType )
    MemberType _member;
};

int testFunction( Type0 )                           { return 0; }
int testFunction( Type1 )                           { return 1; }
int testFunction( Type2 )                           { return 2; }
int testFunction( typename TestClass::MemberType )  { return 3; }


template <class T, class TT>
concept ConvertibleTo
= requires ( T a, TT b )
{
    { TT(a) };
    { b = a };
};

template <class T, class TT>
bool convertibleTo( T a, TT b )
requires ConvertibleTo<T,TT>
{
    return true;
}

template <class T, class TT>
bool convertibleTo( T a, TT b )
requires (!ConvertibleTo<T,TT>)
{
    return false;
}

} // namespace test



TEST_CASE( "STRONG_TYPEDEF", "[stl_extension]" )
{
    CIE_TEST_CASE_INIT( "STRONG_TYPEDEF" )

    {
        CIE_TEST_CASE_INIT( "global typedef" )
        test::Type0 type0;
        test::Type1 type1;
        test::Type2 type2;

        CHECK( test::testFunction(type0) == 0 );
        CHECK( test::testFunction(type1) == 1 );
        CHECK( test::testFunction(type2) == 2 );

        // TODO: typedefs are still interconvertible -> this check fails
        CHECK( !test::convertibleTo(type2,type1) );
    }

    {
        CIE_TEST_CASE_INIT( "member typedef" )
        typename test::TestClass::MemberType memberType;
        CHECK( test::testFunction(memberType) == 3 );
    }
    
} // TEST_CASE STRONG_TYPEDEF


} // namespace cie::utils