#ifndef LINALG_UTILITIES_CPP
#define LINALG_UTILITIES_CPP

// --- Internal Includes ---
#include "../inc/linalgutilities.hpp"


namespace cie::linalg {


void runtime_check( bool result, const char message[] )
{
    if( !result )
        throw std::runtime_error{ message };
}

    
}

#endif