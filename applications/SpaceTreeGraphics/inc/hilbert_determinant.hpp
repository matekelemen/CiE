#ifndef MATHIEU_STABILITY_HILBERT_DETERMINANT_HPP
#define MATHIEU_STABILITY_HILBERT_DETERMINANT_HPP

// --- Internal Includes ---
#include "../inc/complex.hpp"

namespace cie {
namespace mathieu {


Complex hilbertDeterminant( double delta,
                            double epsilon,
                            double gamma,
                            double omega,
                            double omegaCritical,
                            size_t N );


}
}

#endif