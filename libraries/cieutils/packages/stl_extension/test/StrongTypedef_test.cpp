// --- Internal Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"
#include "cieutils/packages/stl_extension/inc/StrongTypedef.hpp"

// --- STL Includes ---
#include <vector>


namespace cie::utils {


// Define overloaded functions to check types

using VectorBase  = std::vector<int>;
using IntBase     = int;
using PointerBase = int*;

STRONG_TYPEDEF( VectorBase, SubVector1 )
STRONG_TYPEDEF( VectorBase, SubVector2 )

STRONG_TYPEDEF( IntBase, SubInt1 )
STRONG_TYPEDEF( IntBase, SubInt2 )

STRONG_TYPEDEF( PointerBase, SubPointer1 )
STRONG_TYPEDEF( PointerBase, SubPointer2 )

struct TestClass
{
    STRONG_TYPEDEF( VectorBase, MemberVector )
    STRONG_TYPEDEF( IntBase, MemberInt )
    STRONG_TYPEDEF( PointerBase, MemberPointer )
};

int testFunction( VectorBase value )                { return 0; }
int testFunction( SubVector1 value )                { return 1; }
int testFunction( SubVector2 value )                { return 2; }
int testFunction( TestClass::MemberVector value )   { return 3; }

int testFunction( IntBase value )                   { return 4; }
int testFunction( SubInt1 value )                   { return 5; }
int testFunction( SubInt2 value )                   { return 6; }
int testFunction( TestClass::MemberInt value )      { return 7; }

int testFunction( PointerBase value )               { return 8; }
int testFunction( SubPointer1 value )               { return 9; }
int testFunction( SubPointer2 value )               { return 10; }
int testFunction( TestClass::MemberPointer value )  { return 11; }


template <class T, class TT>
concept ConvertibleTo
= requires ( T a, TT b )
{
    { TT(a) };
    { b = a };
};

template <class T, class TT>
concept NotConvertibleTo
= !ConvertibleTo<T,TT>;

template <class T, class TT>
bool convertibleTo( T a, TT b )
requires ConvertibleTo<T,TT>
{
    return true;
}

template <class T, class TT>
bool convertibleTo( T a, TT b )
requires NotConvertibleTo<T,TT>
{
    return false;
}







CIE_TEST_CASE( "STRONG_TYPEDEF", "[stl_extension]" )
{
    CIE_CIE_TEST_CASE_INIT( "STRONG_TYPEDEF" )

    {
        CIE_CIE_TEST_CASE_INIT( "std::vector" )
        VectorBase              base;
        SubVector1              sub1;
        SubVector2              sub2;
        TestClass::MemberVector member;

        SubVector1 tmp(base);

        CHECK( testFunction(base) == 0 );
        CHECK( testFunction(sub1) == 1 );
        CHECK( testFunction(sub2) == 2 );
        CHECK( testFunction(member) == 3 );

        CHECK( convertibleTo(base, sub1) );
        CHECK( convertibleTo(sub1, base) );
        CHECK( !convertibleTo(sub1, sub2) );
    }

    {
        CIE_CIE_TEST_CASE_INIT( "int" )
        IntBase              base = 0;
        SubInt1              sub1;
        SubInt2              sub2;
        TestClass::MemberInt member;

        CHECK( testFunction(base) == 4 );
        CHECK( testFunction(sub1) == 5 );
        CHECK( testFunction(sub2) == 6 );
        CHECK( testFunction(member) == 7 );

        CHECK( convertibleTo(base, sub1) );
        CHECK( convertibleTo(sub1, base) );
        CHECK( !convertibleTo(sub1, sub2) );
    }

    {
        CIE_CIE_TEST_CASE_INIT( "int*" )
        PointerBase              base = nullptr;
        SubPointer1              sub1;
        SubPointer2              sub2;
        TestClass::MemberPointer member;

        CHECK( testFunction(base) == 8 );
        CHECK( testFunction(sub1) == 9 );
        CHECK( testFunction(sub2) == 10 );
        CHECK( testFunction(member) == 11 );

        CHECK( convertibleTo(base, sub1) );
        CHECK( convertibleTo(sub1, base) );
        CHECK( !convertibleTo(sub1, sub2) );
    }
    
} // CIE_TEST_CASE STRONG_TYPEDEF


} // namespace cie::utils