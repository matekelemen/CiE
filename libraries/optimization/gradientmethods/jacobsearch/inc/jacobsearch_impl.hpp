#ifndef OPTIMIZATION_JACOB_SEARCH_IMPL_HPP
#define OPTIMIZATION_JACOB_SEARCH_IMPL_HPP

#include "linalgtypes.hpp"
#include "linalgoverloads.hpp"

namespace opt {


template <size_t N>
JacobSearch<N>::JacobSearch(    const ObjectivePtr<RNRElement<N>& objective, 
                                const RNRElement<N>& initialPoint,
                                double gradientEstimatorStepSize=1e-10) :
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


template <size_t N>
void JacobSearch<N>::setGradientEstimatorStepSize(double stepSize)
{
    _gradientEstimatorStepSize = stepSize;
}


} // namespace opt

#endif