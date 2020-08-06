// --- Utility Includes ---
#include <cieutils/macros.hpp>

// --- Internal Includes ---
#include "../inc/eigendecomposition.hpp"
#include "../../types/inc/typeoperations.hpp"
#include "../../overloads/inc/vectoroperators.hpp"
#include "../../overloads/inc/matrixoperators.hpp"

namespace cie::linalg {

// Get principal component by the power iteration method
DoubleVector principalComponent(const Matrix<Double>& matrix, Double tolerance, Size maxIterations)
{
    // Check if matrix is rectangular
    if (matrix.size1() != matrix.size2())
        CIE_THROW( std::runtime_error, "Cannot perform eigendecomposition on a non-rectangular matrix!" )

    // Create initial vector 
    DoubleVector memory(matrix.size2(), RAND_MAX/2.0);
    DoubleVector output(matrix.size2());
    for (auto it=output.begin(); it!=output.end(); ++it){
        (*it) = rand()/(Double)RAND_MAX;
    }
    normalize<DoubleVector>(output);

    // Iterative approximation
    Size iteration = 0;
    while ( norm<DoubleVector>( ::operator-(output,memory) )>tolerance && iteration++<maxIterations ){
        memory  = output;
        output  = matrix * output;
        normalize<DoubleVector>(output);
    }

    if (iteration == maxIterations-1)
        throw output;

    return output;
}

} // namespace cie::linalg