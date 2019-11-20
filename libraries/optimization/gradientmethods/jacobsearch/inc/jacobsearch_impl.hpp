#ifndef OPTIMIZATION_JACOB_SEARCH_IMPL_HPP
#define OPTIMIZATION_JACOB_SEARCH_IMPL_HPP

#include "linalgtypes.hpp"
#include "linalgoverloads.hpp"
#include "quadraticsubstitute.hpp"

namespace cie {
namespace opt {


template <size_t N>
JacobSearch<N>::JacobSearch(		const RNRObjectivePtr<N>& objective,
									const RNRElement<N>& initialPoint,
									double samplingStepSize,
									double gradientEstimatorStepSize) :
    _objective(objective),
    _solution(initialPoint),
    _stepCount(0),
    _gradientEstimatorStepSize(gradientEstimatorStepSize)
{
    _solution.setObjective( _objective(_solution) );
}


template <size_t N>
void JacobSearch<N>::step()
{
    // Get gradient
    RNRElement<N> gradient = gradientEstimate<N>(   _solution, 
                                                    _objective, 
                                                    _gradientEstimatorStepSize );
    linalg::normalize<DoubleArray<N>>(gradient.getData());

    // Get samples in the gradient direction
    /*
    RNRElement<N>   point1(_solution.getData() - _samplingStepSize * gradient);
    RNRElement<N>   point2(_solution.getData() + _samplingStepSize * gradient);

    point1.setObjective(_objective(point1.getData()));
    point2.setObjective(_objective(point2.getData()));

	// Create a substitute
    QuadraticSubstitute<N> substitute(  {   &_solution.getData(),
                                            &point1.getData(),
                                            &point2.getData()},
                                        {   _solution.getObjective,
                                            point1.getObjective(),
                                            point2.getObjective()   } );

    // Find minimum of the substitute
    auto result = substitute.minimum();
    RNRElement<N> solutionCandidate(result.first);
    */
}


template <size_t N>
const RNRElement<N>& JacobSearch<N>::getSolution() const
{
    return _solution;
}


template <size_t N>
size_t JacobSearch<N>::stepCount() const 
{
    return _stepCount;
}


template <size_t N>
void JacobSearch<N>::setObjective(ObjectivePtr<RNRElement<N>,double> objective)
{
    _objective = objective;
}


template <size_t N>
void JacobSearch<N>::resetStepCount()
{
    _stepCount = 0;
}


template<size_t N>
void JacobSearch<N>::setSamplingStepSize(double stepSize)
{
	_samplingStepSize = stepSize;
}


template <size_t N>
void JacobSearch<N>::setGradientEstimatorStepSize(double stepSize)
{
    _gradientEstimatorStepSize = stepSize;
}


} // namespace opt
}

#endif