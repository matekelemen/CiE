#ifndef CIE_CIEUTILS_MACROS_HPP
#define CIE_CIEUTILS_MACROS_HPP

// --- Internal Includes ---
#include "../../exceptions/inc/exception.hpp"
#include "cmake_variables.hpp"

// --- STD Includes ---
#include <sstream>


/* The preprocessor variables can be defined/undefined through CMake */


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