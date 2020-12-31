#ifndef CIE_OPTIMIZATION_GRADIENT_ESTIMATE_HPP
#define CIE_OPTIMIZATION_GRADIENT_ESTIMATE_HPP

// --- Linalg Includes ---
#include "linalg/types.hpp"

// --- Internal Incldudes ---
#include "optimization/packages/representation/inc/element.hpp"
#include "optimization/packages/objective/inc/objectivefunction.hpp"

namespace cie::opt {


template <size_t N>
DoubleArray<N> gradientEstimate(    const RNRElement<N>& point,
                                    RNRObjectivePtr<N>& objective, 
                                    const RNRElement<N>& stepSize );


template <size_t N>
DoubleArray<N> gradientEstimate(    const RNRElement<N>& point,
                                    RNRObjectivePtr<N>& objective, 
                                    double stepSize=1e-10 );


} // namespace cie::opt

#include "optimization/packages/gradientdescent/impl/gradientestimate_impl.hpp"

#endif