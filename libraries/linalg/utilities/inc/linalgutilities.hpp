#ifndef LINALG_UTILITIES_HPP
#define LINALG_UTILITIES_HPP

#include "../../types/inc/matrix.hpp"
//#include "../../overloads/inc/vectoroperators.hpp"
//#include "../../overloads/inc/matrixoperators.hpp"
#include "errortypes.hpp"

namespace cie {
namespace linalg {


// Euclidean norms
/*
double norm( const Vector& vector );
double norm( const Matrix& matrix );

void normalize( Vector& vector );
void normalize( Matrix& matrix );
*/

using PermutationVector = std::vector<size_t>;

void updatePermutation( const Matrix& matrix,
                        PermutationVector& permutation,
                        size_t index,
                        double singularTolerance );

void runtime_check( bool result, const char message[] );


} // namespace linalg
}


#endif