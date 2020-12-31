#ifndef CIE_LINALG_HELPER_HPP
#define CIE_LINALG_HELPER_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/container_concepts.hpp"
#include "cieutils/packages/types/inc/types.hpp"

// --- Internal Includes ---
#include "linalg/packages/types/inc/vectortypes.hpp"
#include "linalg/packages/types/inc/matrix.hpp"

// --- STL Includes ---
#include <iomanip>
#include <ostream>
#include <stdexcept>

namespace cie::linalg {
namespace linalghelper {


template<class RowFunction>
void writeRow( const RowFunction& vector, Size size, std::ostream& out, Size digits );

template<concepts::NumericContainer ContainerType>
void write( const ContainerType& vector, std::ostream& out = std::cout );

template<concepts::NumericType ValueType>
void write( const Matrix<ValueType>& matrix, std::ostream& out = std::cout );


} // namespace linalghelper
} // namespace cie::linalg

#include "linalg/packages/utilities/impl/linalghelper_impl.hpp"

#endif