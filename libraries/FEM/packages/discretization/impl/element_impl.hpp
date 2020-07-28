#ifndef CIE_FEM_ELEMENT_IMPL_HPP
#define CIE_FEM_ELEMENT_IMPL_HPP

namespace cie::fem {


template <  Size Dimension,
            concepts::NumericType NT >
AbsElement<Dimension,NT>::AbsElement() :
    _basis( std::make_shared<LinearBasisFunctionSet<Dimension,NT>>() )
{
}


template <  Size Dimension,
            concepts::NumericType NT >
template <class CoefficientContainer, class PointType>
NT
AbsElement<Dimension,NT>::operator()(   const CoefficientContainer& coefficients,
                                        const PointType& point ) const
requires concepts::ClassContainer<PointType,NT>
{
    NT value(1.0);
    NT temp;
    auto cSetIt = coefficients.begin();

    for (auto fSetIt=_basis->functions().begin(); fSetIt!=_basis->functions().end(); ++fSetIt,++cSetIt )
    {
        temp = 0.0;
        auto coefIt     = cSetIt->begin();
        auto functionIt = fSetIt->begin();
        for ( ; coefIt!=cSetIt->end(); ++coefIt,++functionIt )
            temp += *coefIt * functionIt->operator()(point);
        value *= temp;
    }
    return value;
}


template <  Size Dimension,
            concepts::NumericType NT >
template <  class CoefficientContainer,
            class OutputIterator,
            template <class ...> class PointContainer, class PointType, class ...Args >
void AbsElement<Dimension,NT>::operator()(  const CoefficientContainer& coefficients
                                            const PointContainer<PointType,Args...>& points,
                                            OutputIterator outputIt )
requires concepts::ClassContainer<CoefficientContainer,NT>
            && concepts::ClassContainer<PointType,NT>
            && concepts::ClassIterator<OutputIterator,NT>
{
    // TODO
    NT value(1.0);
    NT temp;
    auto cSetIt = coefficients.begin();

    for (auto fSetIt=_basis->functions().begin(); fSetIt!=_basis->functions().end(); ++fSetIt,++cSetIt )
    {
        temp = 0.0;
        auto coefIt     = cSetIt->begin();
        auto functionIt = fSetIt->begin();
        for ( ; coefIt!=cSetIt->end(); ++coefIt,++functionIt )
            temp += functionIt->operator()(point);
        value *= temp;
    }
    return value;
}


}

#endif