#ifndef CIE_CIEUTILS_MACRO_DETAILS_HPP
#define CIE_CIEUTILS_MACRO_DETAILS_HPP


// File path and line number
#define CIE_MACRO_TO_STRING_HELPER(num) #num
#define CIE_MACRO_TO_STRING(num) CIE_MACRO_TO_STRING_HELPER(num)
#define CIE_CODE_LOCATION "in file\t\t: " __FILE__ "\nin line\t\t: " CIE_MACRO_TO_STRING(__LINE__)


#endif