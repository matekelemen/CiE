#ifndef LINALG_HELPER_HPP
#define LINALG_HELPER_HPP

#include <iomanip>
#include <ostream>
#include <stdexcept>

#include "../../types/inc/vectortypes.hpp"
#include "../../types/inc/matrix.hpp"

namespace cie {
namespace linalg {
namespace linalghelper {


template<typename DoubleVectorType>
void writeRow( const DoubleVectorType& vector, size_t size, std::ostream& out, size_t digits );


// Write to an ostream, default argument is std::cout
void write( const DoubleVector& vector, std::ostream& out = std::cout );
void write( const Matrix& matrix, std::ostream& out = std::cout );


} // namespace linalghelper
} // namespace linalg
}

#include "linalghelper_impl.hpp"

#endif