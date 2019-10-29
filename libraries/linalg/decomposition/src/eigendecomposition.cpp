#include "../inc/eigendecomposition.hpp"

namespace linalg {

// Get principal component by the power iteration method
Vector principalComponent(const Matrix& matrix, double tolerance, size_t maxIterations)
{
    // Check if matrix is rectangular
    if (matrix.size1() != matrix.size2())
        throw std::runtime_error("Cannot perform eigendecomposition on a non-rectangular matrix!");

    // Create initial vector 
    Vector memory(matrix.size2(), RAND_MAX/2.0);
    Vector output(matrix.size2());
    for (auto it=output.begin(); it!=output.end(); ++it){
        (*it) = rand()/(double)RAND_MAX;
    }
    normalize(output);

    // Iterative approximation
    size_t iteration = 0;
    while ( norm(output-memory)>tolerance && iteration<maxIterations ){
        memory  = output;
        output  = matrix * output;
        normalize(output);
        ++iteration;
    }

    if (iteration == maxIterations-1)
        throw output;

    return output;
}

}