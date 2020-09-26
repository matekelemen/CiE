#ifndef CIE_CIEUTILS_ASSERTION_MACROS_HPP
#define CIE_CIEUTILS_ASSERTION_MACROS_HPP

// --- Internal Includes ---
#include "cieutils/packages/exceptions/inc/exception.hpp"
#include "cieutils/packages/macros/inc/detail.hpp"
#include "cmake_variables.hpp"

// --- STL Includes ---
#include <sstream>
#include <cassert>

// Assertion message
#define CIE_ASSERTION_MESSAGE(expression) "\nCIE assertion failure!\nassertion\t: " CIE_MACRO_TO_STRING(expression) "\n" CIE_CODE_LOCATION

// Static assertion
#define CIE_ASSERT(boolExpression,message)                                  \
    assert( (boolExpression) && message != std::to_string(0) );

// Static assertion
#define CIE_STATIC_ASSERT(boolExpression)                                   \
    static_assert(boolExpression, CIE_ASSERTION_MESSAGE(boolExpression));


#endif