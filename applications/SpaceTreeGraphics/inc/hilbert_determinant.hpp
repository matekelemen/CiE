#ifndef CIE_SPACE_TREE_GRAPHICS_HILBERT_DETERMINANT_HPP
#define CIE_SPACE_TREE_GRAPHICS_HILBERT_DETERMINANT_HPP

// --- Utility Includes ---
#include <cieutils/types.hpp>

// --- STL Includes ---
#include <complex>

namespace cie {


using Complex = std::complex<double>;


Complex hilbertDeterminant( double delta,
                            double epsilon,
                            double gamma,
                            double omega,
                            double omegaCritical,
                            Size N );


} // namespace cie

#endif