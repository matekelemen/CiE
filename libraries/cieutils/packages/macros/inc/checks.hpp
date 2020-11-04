#ifndef CIE_CIEUTILS_CHECK_MACROS_HPP
#define CIE_CIEUTILS_CHECK_MACROS_HPP

// --- Internal Includes ---
#include "cieutils/packages/exceptions/inc/exception.hpp"
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/macros/inc/detail.hpp"
#include "cmake_variables.hpp"

// --- STL Includes ---
#include <sstream>


/* The preprocessor variables can be defined/undefined through CMake */


#define CIE_CHECK( boolExpression, message )                        \
    if ( !boolExpression )                                          \
        CIE_THROW( Exception, message )


#define CIE_CHECK_POINTER( pointer )                                                \
    if (pointer == nullptr)                                                         \
    {                                                                               \
        CIE_THROW( cie::NullPtrException, "" )                                      \
    }


#ifdef CIE_ENABLE_OUT_OF_RANGE_TESTS
    #define CIE_OUT_OF_RANGE_CHECK(boolExpression)                  \
        if (!(boolExpression))                                      \
        {                                                           \
            std::stringstream stream;                               \
            stream << #boolExpression;                              \
            CIE_THROW( cie::OutOfRangeException, stream.str() );    \
        }
#else
    #define CIE_OUT_OF_RANGE_CHECK(boolExpression)
#endif


#ifdef CIE_ENABLE_DIVISION_BY_ZERO_CHECKS
    #define CIE_DIVISION_BY_ZERO_CHECK(boolExpression)                  \
        if (!(boolExpression))                                          \
        {                                                               \
            std::stringstream stream;                                   \
            stream << #boolExpression;                                  \
            CIE_THROW( cie::DivisionByZeroException, stream.str() );    \
        }
#else
    #define CIE_DIVISION_BY_ZERO_CHECK(boolExpression)
#endif


#ifdef CIE_ENABLE_RUNTIME_GEOMETRY_CHECKS
    #define CIE_RUNTIME_GEOMETRY_CHECK(boolExpression, message)                 \
        if (!(boolExpression))                                                  \
        {                                                                       \
            std::stringstream stream;                                           \
            stream << #boolExpression;                                          \
            stream << " (";                                                     \
            stream << message;                                                  \
            stream << ")";                                                      \
            CIE_THROW( cie::GeometryException, stream.str() );                  \
        }
#else
    #define CIE_RUNTIME_GEOMETRY_CHECK(boolExpression, message)
#endif


#endif