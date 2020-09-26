#ifndef CIE_LINALG_ERROR_TYPES_HPP
#define CIE_LINALG_ERROR_TYPES_HPP

// --- Utility Includes ---
#include "cieutils/packages/exceptions/inc/exception.hpp"
#include "cieutils/packages/types/inc/types.hpp"

// --- Internal Includes ---
#include "linalg/packages/utilities/inc/linalghelper.hpp"

namespace cie::linalg {

template <class MatrixType>
class MatrixError : public Exception {
public:
    MatrixError(const String& message, const MatrixType& matrix);
};

}

#include "linalg/packages/utilities/impl/errortypes_impl.hpp"

#endif