#ifndef LINALG_UTILITIES_CPP
#define LINALG_UTILITIES_CPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- Internal Includes ---
#include "linalg/packages/utilities/inc/linalgutilities.hpp"


namespace cie::linalg {


void runtime_check( bool result, const char message[] )
{
    if( !result )
        CIE_THROW( std::runtime_error, message )
}

    
} // namespace cie::linalg

#endif