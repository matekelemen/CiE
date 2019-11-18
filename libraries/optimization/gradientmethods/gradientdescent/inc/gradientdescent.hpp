#ifndef OPTIMIZATION_GRADIENT_DESCENT_HPP
#define OPTIMIZATION_GRADIENT_DESCENT_HPP

#include "../../solver/inc/solver.hpp"
#include "../../objective/inc/objectivefunction.hpp"
#include <functional>

namespace opt {


// Valid elements are : 
// - RNRElement
// - RNRVectorElement
template<size_t N>
class GradientDescent : public OptimizationMethod {
public:
    GradientDescent(    const ObjectivePtr<RNRElement<N>,double>& objective, 
                        const RNRElement<N>& initialPoint, 
                        const std::function<RNRElement<N>(const GradientDescent<N>&)> stepSizeDeterminor );

    GradientDescent(    const ObjectivePtr<RNRElement<N>,double>& objective,
                        const RNRElement<N>& initialPoint);

    void step();

    const RNRElement<N>& getSolution() const;
    size_t stepCount() const;
    const std::function<RNRElement<N>(const GradientDescent&)> stepSizeDeterminor() const;

    void setObjective(ObjectivePtr<RNRElement<N>,double> objective);
    void resetStepCount();

private:
    ObjectivePtr<RNRElement<N>,double> _objective;
    RNRElement<N> _solution;
    size_t _stepCount;
    std::function<RNRElement<N>(const GradientDescent&)> _stepSizeDeterminor;
};


template <size_t N>
RNRElement<N> defaultStepSizeDeterminor(const GradientDescent<N>& solver);


} // namespace opt

#include "gradientdescent_impl.hpp"

#endif