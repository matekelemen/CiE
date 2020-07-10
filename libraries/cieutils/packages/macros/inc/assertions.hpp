#ifndef CIE_CIEUTILS_ASSERTION_MACROS_HPP
#define CIE_CIEUTILS_ASSERTION_MACROS_HPP

// --- Internal Includes ---
#include "../../exceptions/inc/exception.hpp"
#include "cmake_variables.hpp"

// --- STL Includes ---
#include <sstream>
#include <cassert>


// File path and line number
#define CIE_MACRO_TO_STRING_HELPER(num) #num
#define CIE_MACRO_TO_STRING(num) CIE_MACRO_TO_STRING_HELPER(num)
#define CIE_CODE_LOCATION "in file\t\t: " __FILE__ "\nin line\t\t: " CIE_MACRO_TO_STRING(__LINE__)

// Assertion message
#define CIE_ASSERTION_MESSAGE(expression) "\nCIE assertion failure!\nassertion\t: " CIE_MACRO_TO_STRING(expression) "\n" CIE_CODE_LOCATION


// Static assertion
#define CIE_ASSERT(boolExpression)                                          \
    assert(boolExpression);

// Static assertion
#define CIE_STATIC_ASSERT(boolExpression)                                   \
    static_assert(boolExpression, CIE_ASSERTION_MESSAGE(boolExpression));


#endif