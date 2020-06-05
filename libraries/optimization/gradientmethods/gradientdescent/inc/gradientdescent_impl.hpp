#ifndef OPTIMIZATION_GRADIENT_DESCENT_IMPL_HPP
#define OPTIMIZATION_GRADIENT_DESCENT_IMPL_HPP

// --- Linalg Includes ---
#include "linalg/types.hpp"
#include "linalg/overloads.hpp"

// --- Internal Includes ---
#include "gradientestimate.hpp"
#include "../../solver/inc/solver.hpp"

// --- STD Includes ---
#include <functional>

namespace cie {
namespace opt {

template <size_t N>
GradientDescent<N>::GradientDescent(  const ObjectivePtr<RNRElement<N>, double>& objective,
                                      const RNRElement<N>& initialPoint, 
                                      const std::function<RNRElement<N>(const GradientDescent<N>&)> stepSizeDeterminor ) :
    _objective(objective),
    _solution(initialPoint),
    _stepCount(0),
    _stepSizeDeterminor(stepSizeDeterminor)
{
    _solution.setObjective( _objective(_solution) );
}


template <size_t N>
GradientDescent<N>::GradientDescent(    const ObjectivePtr<RNRElement<N>, double>& objective,
                                        const RNRElement<N>& initialPoint ) :
    _objective(objective),
    _solution(initialPoint),
    _stepCount(0),
    _stepSizeDeterminor(defaultStepSizeDeterminor<N>)
{
    _solution.setObjective( _objective(_solution) );
}


template <size_t N>
void GradientDescent<N>::step()
{
    // Get step size
    _stepCount++;
    RNRElement<N> stepSize = _stepSizeDeterminor(*this);

    // Get gradient
    RNRElement<N> gradient = gradientEstimate<N>( _solution, _objective, 1e-10 );
    linalg::normalize<DoubleArray<N>>(gradient.getData());

    // Apply step size
    for (size_t i=0; i<N; ++i)
    {
        _solution.getData()[i] = _solution.getData()[i] - gradient.getData()[i]*stepSize.getData()[i];
    }

    // Appply step
    //_solution.getData() = _solution.getData() + gradient.getData();
    _solution.setObjective( _objective(_solution) );
}


template <size_t N>
const RNRElement<N>& GradientDescent<N>::getSolution() const
{
    return _solution;
}


template <size_t N>
size_t GradientDescent<N>::stepCount() const
{
    return _stepCount;
}


template <size_t N>
const std::function<RNRElement<N>(const GradientDescent<N>&)> GradientDescent<N>::stepSizeDeterminor() const
{
    return _stepSizeDeterminor;
}


template <size_t N>
void GradientDescent<N>::setObjective(ObjectivePtr<RNRElement<N>,double> objective)
{
    _objective = objective;
}


template <size_t N>
void GradientDescent<N>::resetStepCount()
{
    _stepCount = 0;
}




template <size_t N>
RNRElement<N> defaultStepSizeDeterminor(const GradientDescent<N>& solver)
{
    RNRElement<N> stepSize;
    stepSize.getData().fill( 1.0/solver.stepCount() );
    return stepSize;
}


} // namespace opt
}

#endif