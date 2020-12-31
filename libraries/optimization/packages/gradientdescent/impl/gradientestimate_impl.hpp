#ifndef CIE_OPTIMIZATION_GRADIENT_ESTIMATE_IMPL_HPP
#define CIE_OPTIMIZATION_GRADIENT_ESTIMATE_IMPL_HPP

namespace cie::opt {


template <size_t N>
DoubleArray<N> gradientEstimate(            const RNRElement<N>& point,
                                            RNRObjectivePtr<N>& objective, 
                                            const RNRElement<N>& stepSize )
{
    // Check point size
    if (point.getData().size() != N) 
        throw std::runtime_error("Data size mismatch!");

    // Allocate
    DoubleArray<N> gradient;
    RNRElement<N> assert = point;
    double step = 0.0;

    // Get gradient estimate
    for (size_t componentIndex=0; componentIndex<N; ++componentIndex)
    {
        step = stepSize.getData()[componentIndex];
        if ( std::abs(step) < 1e-16 )
            throw std::runtime_error("Cannot compute gradient with 0 component step size");

        assert.getData()[componentIndex] += step;
        gradient[componentIndex] = ( objective(assert) - point.getObjective() ) / step;
        assert.getData()[componentIndex] -= step;
    }

    return gradient;
}


template <size_t N>
DoubleArray<N> gradientEstimate(    const RNRElement<N>& point,
                                    RNRObjectivePtr<N>& objective, 
                                    double stepSize )
{
    RNRElement<N> step;
    step.getData().fill(stepSize);
    return gradientEstimate( point, objective, step);
}


} // namespace cie::opt

#endif