#ifndef OPTIMIZATION_GRADIENT_ESTIMATE_HPP
#define OPTIMIZATION_GRADIENT_ESTIMATE_HPP

// --- Linalg Includes ---
#include "linalg/types.hpp"

// --- Internal Incldudes ---
#include "../../representation/inc/element.hpp"
#include "objectivefunction.hpp"

namespace cie {
namespace opt {


template <size_t N>
DoubleArray<N> gradientEstimate(    const RNRElement<N>& point,
                                    RNRObjectivePtr<N>& objective, 
                                    const RNRElement<N>& stepSize );


template <size_t N>
DoubleArray<N> gradientEstimate(    const RNRElement<N>& point,
                                    RNRObjectivePtr<N>& objective, 
                                    double stepSize=1e-10 );


} // namespace opt
}

#include "gradientestimate_impl.hpp"

#endif