#ifndef CIE_LINALG_ERROR_TYPES_HPP
#define CIE_LINALG_ERROR_TYPES_HPP

// --- Utility Includes ---
#include "cieutils/packages/exceptions/inc/exception.hpp"
#include "cieutils/packages/types/inc/types.hpp"

// --- Internal Includes ---
#include "linalg/packages/utilities/inc/linalghelper.hpp"

namespace cie::linalg {

class MatrixError : public Exception {
public:
    MatrixError( const std::string& r_location,
                 const std::string& r_message );
};

} // namespace cie::linalg

#endif