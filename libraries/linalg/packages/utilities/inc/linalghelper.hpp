#ifndef CIE_LINALG_HELPER_HPP
#define CIE_LINALG_HELPER_HPP

// --- Utility Includes ---
#include <cieutils/concepts.hpp>
#include <cieutils/types.hpp>

// --- Internal Includes ---
#include "../../types/inc/vectortypes.hpp"
#include "../../types/inc/matrix.hpp"

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

#include "../impl/linalghelper_impl.hpp"

#endif