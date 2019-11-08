#ifndef LINALG_HELPER_HPP
#define LINALG_HELPER_HPP

#include <iomanip>
#include <ostream>
#include <stdexcept>

#include "../../types/inc/matrix.hpp"

namespace linalg {
namespace linalghelper {


template<typename VectorType>
void writeRow( const VectorType& vector, size_t size, std::ostream& out, size_t digits );


// Write to an ostream, default argument is std::cout
void write( const Vector& vector, std::ostream& out = std::cout );
void write( const Matrix& matrix, std::ostream& out = std::cout );


} // namespace linalghelper
} // namespace linalg

#include "linalghelper_impl.hpp"

#endif