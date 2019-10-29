#ifndef LINALG_UTILITIES_HPP
#define LINALG_UTILITIES_HPP

#include "../../types/inc/matrix.hpp"
#include "../../overloads/inc/vectoroperators.hpp"
#include "../../overloads/inc/matrixoperators.hpp"

#include <iomanip>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <numeric>

namespace linalg {

// Write to an ostream, default argument is std::cout
void write( const Vector& vector, std::ostream& out = std::cout );
void write( const Matrix& matrix, std::ostream& out = std::cout );

// Euclidean norms
double norm( const Vector& vector );
double norm( const Matrix& matrix );

void normalize( Vector& vector );
void normalize( Matrix& matrix );



namespace linalghelper {

using PermutationVector = std::vector<size_t>;

void updatePermutation( const Matrix& matrix,
                        PermutationVector& permutation,
                        size_t index,
                        double singularTolerance );

void runtime_check( bool result, const char message[] );

template<typename VectorType>
void writeRow( const VectorType& vector, size_t size, std::ostream& out, size_t digits );

} // namespace linalghelper

} // namespace linalg

#include "linalgutilities_impl.hpp"

#endif