#ifndef CIE_CIEUTILS_MACROS_HPP
#define CIE_CIEUTILS_MACROS_HPP

// --- Internal Includes ---
#include "../../exceptions/inc/exception.hpp"
#include "cmake_variables.hpp"

// --- STD Includes ---
#include <sstream>


/* The preprocessor variables can be defined/undefined through CMake */


#ifdef CIE_ENABLE_OUT_OF_RANGE_TESTS
    #define CIE_OUT_OF_RANGE_TEST(boolExpression, objectName)   \
        if (!(boolExpression))                                  \
        {                                                       \
            std::stringstream stream;                           \
            stream << #boolExpression;                          \
            stream << " (";                                     \
            stream << objectName;                               \
            stream << ")";                                      \
            throw cie::OutOfRangeException( stream.str() );     \
        }
#else
    #define CIE_OUT_OF_RANGE_TEST(boolExpression, objectName)
#endif


#endif