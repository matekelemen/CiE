#ifndef OPTIMIZATION_JACOB_SEARCH_IMPL_HPP
#define OPTIMIZATION_JACOB_SEARCH_IMPL_HPP

// --- Linalg Includes ---
#include "linalg/packages/types/inc/arraytypes.hpp"
#include "linalg/packages/overloads/inc/vectoroperators.hpp"

// --- Internal Includes ---
#include "optimization/packages/jacobsearch/inc/quadraticsubstitute.hpp"

namespace cie::opt {


template <size_t N>
JacobSearch<N>::JacobSearch(		const RNRObjectivePtr<N>& objective,
									const RNRElement<N>& initialPoint,
									double samplingStepSize,
									double gradientEstimatorStepSize) :
    _objective(objective),
    _solution(initialPoint),
    _stepCount(0),
    _samplingStepSize(samplingStepSize),
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
    
    RNRElement<N>   point1(_solution.getData() - _samplingStepSize * gradient.getData());
    RNRElement<N>   point2(_solution.getData() + _samplingStepSize * gradient.getData());

    point1.setObjective(_objective(point1.getData()));
    point2.setObjective(_objective(point2.getData()));

	// Create a substitute
    QuadraticSubstitute<N> substitute(  {   &point1.getData(),
                                            &_solution.getData(),
                                            &point2.getData()},
                                        {   point1.getObjective(),
                                            _solution.getObjective(),
                                            point2.getObjective()   } );

    // Find minimum of the substitute
    auto result = substitute.minimum();

    _solution.setData( result.first );
    _solution.setObjective( _objective(_solution) );
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


} // namespace cie::opt

#endif