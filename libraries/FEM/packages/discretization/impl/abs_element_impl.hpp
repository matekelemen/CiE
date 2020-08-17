#ifndef CIE_FEM_ABS_ELEMENT_IMPL_HPP
#define CIE_FEM_ELEMENT_IMPL_HPP

// --- Utility Includes ---
#include <cieutils/macros.hpp>
#include <cieutils/exceptions.hpp>
#include <cieutils/concepts.hpp>

// --- STL Includes ---
#include <numeric>


namespace cie::fem {
// ---------------------------------------------------------
// CONSTRUCTORS
// ---------------------------------------------------------
template <class BasisType>
AbsElement<BasisType>::AbsElement( const typename AbsElement<BasisType>::dof_container& dofs ) :
    DoFMap<typename AbsElement<BasisType>::dof_container>(dofs)
{
}


// ---------------------------------------------------------
// BASIS
// ---------------------------------------------------------
template <class BasisType>
inline void
AbsElement<BasisType>::basisProducts(   const typename AbsElement::ansatz_value_container& ansatzValues,
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



template <class BasisType>
inline void
AbsElement<BasisType>::basisDerivativeProducts( const typename AbsElement::ansatz_value_container& ansatzValues,
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

template <class BasisType>
template <class CoefficientContainer, concepts::STLContainer AnsatzContainer>
inline typename AbsElement<BasisType>::NT
AbsElement<BasisType>::operator()(  const CoefficientContainer& coefficients,
                                    const AnsatzContainer& ansatzValues ) const
requires concepts::ClassContainer<CoefficientContainer,NT>
            && concepts::ClassContainer<typename AnsatzContainer::value_type,NT>
{
    // Get basis values
    typename AbsElement::value_container basis;
    this->basisProducts( ansatzValues, basis );

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


template <class BasisType>
template <class CoefficientContainer>
inline typename AbsElement<BasisType>::NT
AbsElement<BasisType>::operator()(  const CoefficientContainer& coefficients,
                                    const typename AbsElement<BasisType>::LocalCoordinates& point ) const
requires concepts::ClassContainer<CoefficientContainer,NT>
{
    std::array<std::vector<NT>,dimension>   basisValues;
    auto basisIt = basisValues.begin();

    for (Size dim=0; dim<dimension; ++dim,++basisIt)
        for (const auto& function : this->_basis.functions(dim))
            basisIt->push_back( function(point[dim]) );

    return this->operator()(    coefficients,
                                basisValues );
}


template <class BasisType>
template <  class CoefficientContainer,
            class PointIterator,
            class OutputIterator >
inline void 
AbsElement<BasisType>::operator()(  const CoefficientContainer& coefficients,
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

template <class BasisType>
inline void
AbsElement<BasisType>::_derivative( const coefficient_container& coefficients,
                                    const ansatz_value_container& basisValues,
                                    const ansatz_value_container& derivativeValues,
                                    point_type& gradient )
{
    throw AbstractCallException( "AbsElement::derivative" );
}


template <class BasisType>
inline void
AbsElement<BasisType>::derivative( const coefficient_container& coefficients,
                                    const ansatz_value_container& basisValues,
                                    const ansatz_value_container& derivativeValues,
                                    point_type& gradient )
{
    return this->_derivative(   coefficients,
                                basisValues,
                                gradient );
}


template <class BasisType>
inline typename AbsElement<BasisType>::point_type
AbsElement<BasisType>::derivative(  const coefficient_container& coefficients,
                                    const LocalCoordinates& point )
{
    std::array<std::vector<NT>,dimension>   basisValues, derivativeValues;
    auto                                    _derivatives = _basis.derivatives();
    point_type                              gradient;

    auto valueIt    = basisValues.begin();
    for (Size dim=0; dim<dimension; ++dim,++valueIt)
        for (const auto& function : this->_basis.functions(dim))
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


template <class BasisType>
template <  class PointIterator,
            class OutputIterator >
inline void
AbsElement<BasisType>::derivative(  const coefficient_container& coefficients,
                                    PointIterator pointBegin,
                                    PointIterator pointEnd,
                                    OutputIterator outputIt )
requires concepts::ClassIterator<PointIterator,LocalCoordinates>
            && concepts::ClassIterator<OutputIterator,point_type>
{
    auto _derivatives = _basis.derivatives();

    for ( ; pointBegin!=pointEnd; ++pointBegin )
    {
        std::array<std::vector<NT>,dimension>   basisValues, derivativeValues;

        auto valueIt    = basisValues.begin();
        for (Size dim=0; dim<dimension; ++dim,++valueIt)
            for (const auto& function : this->_basis.functions(dim))
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

template <class BasisType>
inline void
AbsElement<BasisType>::localCoordinates(    const typename AbsElement<BasisType>::point_type& point,
                                            typename AbsElement<BasisType>::LocalCoordinates& localPoint ) const
{
    utils::setContainerSize(localPoint,dimension);
    this->toLocalCoordinates( point, localPoint );
}


template <class BasisType>
template <  class PointIterator,
            class LocalIterator >
inline void 
AbsElement<BasisType>::localCoordinates(    PointIterator pointBegin,
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


template <class BasisType>
inline AbsElement<BasisType>::LocalCoordinates
AbsElement<BasisType>::localCoordinates(  const typename AbsElement<BasisType>::point_type& point ) const
{
    AbsElement<BasisType>::LocalCoordinates output;
    utils::setContainerSize(output,dimension);
    this->toLocalCoordinates( point, output );
    return output;
}





// ---------------------------------------------------------
// 1D BASE SPECIALIZATION
// ---------------------------------------------------------

template <class BasisType>
template <class ...Args>
AbsElement1D<BasisType>::AbsElement1D(  const std::pair<typename AbsElement1D::NT, typename AbsElement1D::NT>& domain,
                                        Args&&... args ) :
    AbsElement<BasisType>( std::forward<Args>(args)... ),
    _domain(domain),
    _jacobian( (domain.second-domain.first) / (this->_basis.domain(0,1)-this->_basis.domain(0,0)) ),
    _invJacobian( 1.0/_jacobian )
{
    assert( this->_basis.dimension == 1 );
}


template <class BasisType>
inline void
AbsElement1D<BasisType>::toLocalCoordinates(    const typename AbsElement1D::point_type& point,
                                                typename AbsElement1D::LocalCoordinates& localPoint ) const
{
    CIE_ASSERT(
        (_domain.first <= point[0]) && (point[0] <= _domain.second),
        "Input coordinate " + std::to_string(point[0]) + " outside element domain ["
        + std::to_string(_domain.first) + "," + std::to_string(_domain.second) + "]"
    );
    localPoint[0] = (point[0] - _domain.first) * _invJacobian - this->_basis.domain(0,1);
}


template <class BasisType>
inline typename AbsElement1D<BasisType>::NT
AbsElement1D<BasisType>::toLocalCoordinates( typename AbsElement1D<BasisType>::NT coordinate ) const
{
    CIE_ASSERT(
        _domain.first <= coordinate && coordinate <= _domain.second,
        "Input coordinate " + std::to_string(coordinate) + " outside element domain ["
        + std::to_string(_domain.first) + "," + std::to_string(_domain.second) + "]"
    );
    return (coordinate - _domain.first) * _invJacobian - this->_basis->domain(0,1);
}


template <class BasisType>
void 
AbsElement1D<BasisType>::_derivative(   const typename AbsElement1D::coefficient_container& coefficients,
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