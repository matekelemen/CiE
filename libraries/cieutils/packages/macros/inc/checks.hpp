#ifndef CIE_CIEUTILS_CHECK_MACROS_HPP
#define CIE_CIEUTILS_CHECK_MACROS_HPP

// --- Internal Includes ---
#include "cieutils/packages/exceptions/inc/exception.hpp"
#include "cieutils/packages/macros/inc/detail.hpp"
#include "cmake_variables.hpp"

// --- STL Includes ---
#include <sstream>


/* The preprocessor variables can be defined/undefined through CMake */


#define CIE_CHECK_POINTER( pointer )                                                \
    if (pointer == nullptr)                                                         \
    {                                                                               \
        std::stringstream message;                                                  \
        message << CIE_CODE_LOCATION                                                \
        throw cie::NullPtrException( message.str() );                               \
    }


#ifdef CIE_ENABLE_OUT_OF_RANGE_TESTS
    #define CIE_OUT_OF_RANGE_ASSERT(boolExpression, functionName)   \
        if (!(boolExpression))                                      \
        {                                                           \
            std::stringstream stream;                               \
            stream << #boolExpression;                              \
            stream << " (";                                         \
            stream << functionName;                                 \
            stream << ")";                                          \
            throw cie::OutOfRangeException( stream.str() );         \
        }
#else
    #define CIE_OUT_OF_RANGE_ASSERT(boolExpression, functionName)
#endif


#ifdef CIE_ENABLE_DIVISION_BY_ZERO_CHECKS
    #define CIE_DIVISION_BY_ZERO_ASSERT(boolExpression, functionName)   \
        if (!(boolExpression))                                          \
        {                                                               \
            std::stringstream stream;                                   \
            stream << #boolExpression;                                  \
            stream << " (";                                             \
            stream << functionName;                                     \
            stream << ")";                                              \
            throw cie::DivisionByZeroException( stream.str() );         \
        }
#else
    #define CIE_DIVISION_BY_ZERO_ASSERT(boolExpression, functionName)
#endif


#ifdef CIE_ENABLE_RUNTIME_GEOMETRY_CHECKS
    #define CIE_RUNTIME_GEOMETRY_ASSERT(boolExpression, message, functionName)  \
        if (!(boolExpression))                                                  \
        {                                                                       \
            std::stringstream stream;                                           \
            stream << #boolExpression;                                          \
            stream << " (";                                                     \
            stream << message;                                                  \
            stream << ")";                                                      \
            throw cie::GeometryException( stream.str(), functionName );         \
        }
#else
    #define CIE_RUNTIME_GEOMETRY_ASSERT(boolExpression, message, functionName)
#endif


#endif