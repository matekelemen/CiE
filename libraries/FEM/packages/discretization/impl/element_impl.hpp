#ifndef CIE_FEM_ELEMENT_IMPL_HPP
#define CIE_FEM_ELEMENT_IMPL_HPP

namespace cie::fem {


template <class BasisType>
template <class CoefficientContainer>
typename AbsElement<BasisType>::NT
AbsElement<BasisType>::operator()(  const CoefficientContainer& coefficients,
                                    const typename AbsElement<BasisType>::LocalCoordinates& point ) const
{
    typename AbsElement<BasisType>::NT value(1.0);
    typename AbsElement<BasisType>::NT temp;
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


template <class BasisType>
template <  class CoefficientContainer,
            class OutputIterator >
void AbsElement<BasisType>::operator()( const CoefficientContainer& coefficients
                                        const typename AbsElement<BasisType>::LocalCoordinates& points,
                                        OutputIterator outputIt )
requires concepts::ClassContainer<CoefficientContainer,typename AbsElement<BasisType>::NT>
            && concepts::ClassIterator<OutputIterator,typename AbsElement<BasisType>::NT>
{
    // TODO
    typename AbsElement<BasisType>::NT value(1.0);
    typename AbsElement<BasisType>::NT temp;
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