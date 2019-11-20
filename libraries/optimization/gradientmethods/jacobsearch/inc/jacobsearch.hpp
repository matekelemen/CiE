#ifndef OPTIMIZATION_JACOB_SEARCH_HPP
#define OPTIMIZATION_JACOB_SEARCH_HPP

#include "../../../representation/inc/element.hpp"
#include "../../../objective/inc/objectivefunction.hpp"
#include "../../gradientdescent/inc/gradientestimate.hpp"
#include "../../../solver/inc/solver.hpp"

namespace opt {


template <size_t N>
class JacobSearch : public OptimizationMethod {
public:
	JacobSearch(			const ObjectivePtr<RNRElement<N>, double>& objective,
							const RNRElement<N>& initialPoint,
							double samplingStepSize=1e-5,
							double gradientEstimatorStepSize=1e-10);
    void step();

    const RNRElement<N>& getSolution() const;
    size_t stepCount() const;

    void setObjective(ObjectivePtr<RNRElement<N>,double> objective);
    void resetStepCount();
	void setSamplingStepSize(double stepSize);
    void setGradientEstimatorStepSize(double stepSize);

private:
    ObjectivePtr<RNRElement<N>	_objective;
    RNRElement<N>				_solution;
    size_t						_stepCount;
	double						_samplingStepSize;
    double						_gradientEstimatorStepSize;
};


} // namespace opt

#include "jacobsearch_impl.hpp"

#endif