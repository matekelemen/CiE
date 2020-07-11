#ifndef CIE_LINALG_ERROR_TYPES_HPP
#define CIE_LINALG_ERROR_TYPES_HPP

// --- Utility Includes ---
#include <cieutils/exceptions.hpp>
#include <cieutils/types.hpp>

// --- Internal Includes ---
#include "linalghelper.hpp"

namespace cie::linalg {

template <class MatrixType>
class MatrixError : public Exception {
public:
    MatrixError(const String& message, const MatrixType& matrix);
};

}

#include "../impl/errortypes_impl.hpp"

#endif