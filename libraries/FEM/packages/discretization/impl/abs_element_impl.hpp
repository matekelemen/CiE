#ifndef CIE_FEM_ABS_ELEMENT_IMPL_HPP
#define CIE_FEM_ELEMENT_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/checks.hpp"
#include "cieutils/packages/exceptions/inc/exception.hpp"
#include "cieutils/packages/concepts/inc/container_concepts.hpp"
#include "cieutils/packages/stl_extension/inc/resize.hpp"

// --- STL Includes ---
#include <numeric>


namespace cie::fem {
// ---------------------------------------------------------
// CONSTRUCTORS
// ---------------------------------------------------------
template <class AnsatzType>
AbsElement<AnsatzType>::AbsElement( const typename AbsElement<AnsatzType>::dof_container& dofs ) :
    DoFMap<typename AbsElement<AnsatzType>::dof_container>(dofs)
{
}


// ---------------------------------------------------------
// BASIS
// ---------------------------------------------------------
template <class AnsatzType>
inline void
AbsElement<AnsatzType>::basis(  const typename AbsElement::ansatz_value_container& ansatzValues,
                                typename AbsElement::value_container& outputContainer ) const
{
    Size numberOfBasisValues    = std::accumulate(  ansatzValues.begin(),
                                                    ansatzValues.end(),
                                                    Size(1),
                                                    [](auto lhs, const auto& rhs) -> Size
                                                    {
                                                        return lhs * rhs.size();
                                                    } );

    auto basisIt                = detail::makeTensorProductBasis(ansatzValues);
    utils::setContainerSize( outputContainer, numberOfBasisValues );
    for (auto outputIt=outputContainer.begin(); outputIt!=outputContainer.end(); ++outputIt,++basisIt)
        *outputIt = basisIt.product();
}



template <class AnsatzType>
inline void
AbsElement<AnsatzType>::basisDerivatives(   const typename AbsElement::ansatz_value_container& ansatzValues,
                                            const typename AbsElement::ansatz_value_container& ansatzDerivativeValues,
                                            typename AbsElement::point_container& outputContainer ) const
{
    Size numberOfBasisValues    = std::accumulate(  ansatzValues.begin(),
                                                    ansatzValues.end(),
                                                    Size(1),
                                                    [](auto lhs, const auto& rhs) -> Size
                                                    {
                                                        return lhs * rhs.size();
                                                    } );
    auto basisDerivativeIt = detail::makeTensorProductDerivatives(ansatzValues, ansatzDerivativeValues);

    utils::setContainerSize( outputContainer, numberOfBasisValues );
    for (Size basisIndex=0; basisIndex<numberOfBasisValues; ++basisIndex,++basisDerivativeIt)
        outputContainer[basisIndex]    = basisDerivativeIt.product();
}


// ---------------------------------------------------------
// COMPUTE FIELD VALUES
// ---------------------------------------------------------

template <class AnsatzType>
template <class CoefficientContainer, concepts::STLContainer AnsatzContainer>
inline typename AbsElement<AnsatzType>::NT
AbsElement<AnsatzType>::operator()(  const CoefficientContainer& coefficients,
                                    const AnsatzContainer& ansatzValues ) const
requires concepts::ClassContainer<CoefficientContainer,NT>
            && concepts::ClassContainer<typename AnsatzContainer::value_type,NT>
{
    // Get basis values
    typename AbsElement::value_container basis;
    this->basis( ansatzValues, basis );

    CIE_OUT_OF_RANGE_ASSERT(
        coefficients.size() == basis.size(),
        "AbsElement::operator()"
    )

    // Compute inner product
    return std::inner_product(  basis.begin(),
                                basis.end(),
                                coefficients.begin(),
                                0.0 );
}


template <class AnsatzType>
template <class CoefficientContainer>
inline typename AbsElement<AnsatzType>::NT
AbsElement<AnsatzType>::operator()(  const CoefficientContainer& coefficients,
                                    const typename AbsElement<AnsatzType>::LocalCoordinates& point ) const
requires concepts::ClassContainer<CoefficientContainer,NT>
{
    std::array<std::vector<NT>,dimension>   basisValues;
    auto basisIt = basisValues.begin();

    for (Size dim=0; dim<dimension; ++dim,++basisIt)
        for (const auto& function : this->_ansatzSet.functions(dim))
            basisIt->push_back( function(point[dim]) );

    return this->operator()(    coefficients,
                                basisValues );
}


template <class AnsatzType>
template <  class CoefficientContainer,
            class PointIterator,
            class OutputIterator >
inline void 
AbsElement<AnsatzType>::operator()(  const CoefficientContainer& coefficients,
                                    PointIterator pointBegin,
                                    PointIterator pointEnd,
                                    OutputIterator outputIt )
requires concepts::ClassContainer<CoefficientContainer,NT>
            && concepts::ClassIterator<PointIterator,LocalCoordinates>
            && concepts::ClassIterator<OutputIterator,NT>
{
    for ( ; pointBegin!=pointEnd; pointBegin++)
        *outputIt++ = this->operator()( coefficients,
                                        *pointBegin );
}


// ---------------------------------------------------------
// COMPUTE FIELD DERIVATIVES
// ---------------------------------------------------------

template <class AnsatzType>
inline void
AbsElement<AnsatzType>::_derivative( const coefficient_container& coefficients,
                                    const ansatz_value_container& basisValues,
                                    const ansatz_value_container& derivativeValues,
                                    point_type& gradient )
{
    throw AbstractCallException( "AbsElement::derivative" );
}


template <class AnsatzType>
inline void
AbsElement<AnsatzType>::derivative( const coefficient_container& coefficients,
                                    const ansatz_value_container& basisValues,
                                    const ansatz_value_container& derivativeValues,
                                    point_type& gradient )
{
    return this->_derivative(   coefficients,
                                basisValues,
                                gradient );
}


template <class AnsatzType>
inline typename AbsElement<AnsatzType>::point_type
AbsElement<AnsatzType>::derivative(  const coefficient_container& coefficients,
                                    const LocalCoordinates& point )
{
    std::array<std::vector<NT>,dimension>   basisValues, derivativeValues;
    auto                                    _derivatives = _ansatzSet.derivatives();
    point_type                              gradient;

    auto valueIt    = basisValues.begin();
    for (Size dim=0; dim<dimension; ++dim,++valueIt)
        for (const auto& function : this->_ansatzSet.functions(dim))
            valueIt->push_back( function(point[dim]) );

    valueIt         = derivativeValues.begin();
    for (Size dim=0; dim<dimension; ++dim,++valueIt)
        for (auto function : _derivatives->functions(dim))
            valueIt->push_back( function(point[dim]) );

    this->_derivative(  coefficients,
                        basisValues,
                        derivativeValues,
                        gradient );

    return gradient;
}


template <class AnsatzType>
template <  class PointIterator,
            class OutputIterator >
inline void
AbsElement<AnsatzType>::derivative(  const coefficient_container& coefficients,
                                    PointIterator pointBegin,
                                    PointIterator pointEnd,
                                    OutputIterator outputIt )
requires concepts::ClassIterator<PointIterator,LocalCoordinates>
            && concepts::ClassIterator<OutputIterator,point_type>
{
    auto _derivatives = _ansatzSet.derivatives();

    for ( ; pointBegin!=pointEnd; ++pointBegin )
    {
        std::array<std::vector<NT>,dimension>   basisValues, derivativeValues;

        auto valueIt    = basisValues.begin();
        for (Size dim=0; dim<dimension; ++dim,++valueIt)
            for (const auto& function : this->_ansatzSet.functions(dim))
                valueIt->push_back( function(pointBegin->at(dim)) );

        valueIt         = derivativeValues.begin();
        for (Size dim=0; dim<dimension; ++dim,++valueIt)
            for (auto function : _derivatives->functions(dim))
                valueIt->push_back( function(pointBegin->at(dim)) );

        this->_derivative(  coefficients,
                            basisValues,
                            derivativeValues,
                            *outputIt++ );
    }
}


// ---------------------------------------------------------
// COORDINATE TRANSFORMATION
// ---------------------------------------------------------

template <class AnsatzType>
inline void
AbsElement<AnsatzType>::localCoordinates(    const typename AbsElement<AnsatzType>::point_type& point,
                                            typename AbsElement<AnsatzType>::LocalCoordinates& localPoint ) const
{
    utils::resize(localPoint,dimension);
    this->toLocalCoordinates( point, localPoint );
}


template <class AnsatzType>
template <  class PointIterator,
            class LocalIterator >
inline void 
AbsElement<AnsatzType>::localCoordinates(    PointIterator pointBegin,
                                            PointIterator pointEnd,
                                            LocalIterator localBegin ) const
requires concepts::ClassIterator<PointIterator,point_type>
            && concepts::ClassIterator<LocalIterator,LocalCoordinates>
{
    for ( ; pointBegin!=pointEnd; pointBegin++)
    {
        this->toLocalCoordinates( *pointBegin, *localBegin++ );
    }
}


template <class AnsatzType>
inline AbsElement<AnsatzType>::LocalCoordinates
AbsElement<AnsatzType>::localCoordinates(  const typename AbsElement<AnsatzType>::point_type& point ) const
{
    AbsElement<BasisType>::LocalCoordinates output;
    utils::resize(output,dimension);
    this->toLocalCoordinates( point, output );
    return output;
}





// ---------------------------------------------------------
// 1D BASE SPECIALIZATION
// ---------------------------------------------------------

template <class AnsatzType>
template <class ...Args>
AbsElement1D<AnsatzType>::AbsElement1D(  const std::pair<typename AbsElement1D::NT, typename AbsElement1D::NT>& domain,
                                        Args&&... args ) :
    AbsElement<AnsatzType>( std::forward<Args>(args)... ),
    _domain(domain),
    _jacobian( (domain.second-domain.first) / (this->_ansatzSet.domain(0,1)-this->_ansatzSet.domain(0,0)) ),
    _invJacobian( 1.0/_jacobian )
{
    CIE_STATIC_ASSERT( AnsatzType::dimension == 1 )
}


template <class AnsatzType>
inline void
AbsElement1D<AnsatzType>::toLocalCoordinates(    const typename AbsElement1D::point_type& point,
                                                typename AbsElement1D::LocalCoordinates& localPoint ) const
{
    CIE_ASSERT(
        (_domain.first <= point[0]) && (point[0] <= _domain.second),
        "Input coordinate " + std::to_string(point[0]) + " outside element domain ["
        + std::to_string(_domain.first) + "," + std::to_string(_domain.second) + "]"
    );
    localPoint[0] = (point[0] - _domain.first) * _invJacobian - this->_ansatzSet.domain(0,1);
}


template <class AnsatzType>
inline typename AbsElement1D<AnsatzType>::NT
AbsElement1D<AnsatzType>::toLocalCoordinates( typename AbsElement1D<AnsatzType>::NT coordinate ) const
{
    CIE_ASSERT(
        _domain.first <= coordinate && coordinate <= _domain.second,
        "Input coordinate " + std::to_string(coordinate) + " outside element domain ["
        + std::to_string(_domain.first) + "," + std::to_string(_domain.second) + "]"
    );
    return (coordinate - _domain.first) * _invJacobian - this->_ansatzSet->domain(0,1);
}


template <class AnsatzType>
void 
AbsElement1D<AnsatzType>::_derivative(   const typename AbsElement1D::coefficient_container& coefficients,
                                        const typename AbsElement1D::ansatz_value_container& ansatzValues,
                                        const typename AbsElement1D::ansatz_value_container& ansatzDerivativeValues,
                                        typename AbsElement1D::point_type& gradient )
{
    std::fill(  gradient.begin(),
                gradient.end(),
                0.0 );
    auto basisDerivatives   = detail::makeTensorProductDerivatives(ansatzValues, ansatzDerivativeValues);
    for (const auto& coefficient : coefficients)
    {
        auto basisDerivativeValues  = basisDerivatives.product();
        for (Size dim=0; dim<this->dimension; ++dim)
            gradient[dim] += basisDerivativeValues[dim] * coefficient;

        ++basisDerivatives;
    }

    for (auto& component : gradient)
        component *= _invJacobian;
}



} // namespace cie::fem

#endif