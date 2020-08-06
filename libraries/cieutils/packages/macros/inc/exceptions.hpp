#ifndef CIE_UTILS_EXCEPTION_MACROS_HPP
#define CIE_UTILS_EXCEPTION_MACROS_HPP

// --- Internal Includes ---
#include "../../exceptions/inc/exception.hpp"
#include "./detail.hpp"

// --- STL Includes ---
#include <sstream>


#define CIE_THROW(ExceptionType,message)                                    \
    {                                                                       \
        std::stringstream msg;                                              \
        msg << "Exception of type " << #ExceptionType << " thrown\n";       \
        msg << CIE_CODE_LOCATION << "\n";                                   \
        msg << "with message: " << message;                                 \
        throw ExceptionType( msg.str() );                                   \
    }


#endif