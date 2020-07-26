#ifndef CIE_CIEUTILS_CHECK_MACROS_HPP
#define CIE_CIEUTILS_CHECK_MACROS_HPP

// --- Internal Includes ---
#include "../../exceptions/inc/exception.hpp"
#include "cmake_variables.hpp"
#include "detail.hpp"

// --- STL Includes ---
#include <sstream>
#include <experimental/source_location>


/* The preprocessor variables can be defined/undefined through CMake */


#define CIE_CHECK_POINTER( pointer )                                                \
    if (pointer == nullptr)                                                         \
    {                                                                               \
        std::stringstream message;                                                  \
        auto location = std::experimental::source_location::current();              \
        message << "In file " << location.file_name();                              \
        message << "\n\t in function";                                              \
        message << location.function_name();                                        \
        throw NullPtrException( message.str() );                                    \
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