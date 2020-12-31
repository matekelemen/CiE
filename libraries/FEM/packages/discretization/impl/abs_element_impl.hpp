#ifndef CIE_FEM_ABS_ELEMENT_IMPL_HPP
#define CIE_FEM_ELEMENT_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/checks.hpp"
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/concepts/inc/container_concepts.hpp"
#include "cieutils/packages/stl_extension/inc/resize.hpp"

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
AbsElement<BasisType>::basis(  const typename AbsElement::basis_value_container& basisValues,
                                typename AbsElement::value_container& outputContainer ) const
{
    Size numberOfBasisValues    = std::accumulate(  basisValues.begin(),
                                                    basisValues.end(),
                                                    Size(1),
                                                    [](auto lhs, const auto& rhs) -> Size
                                                    {
                                                        return lhs * rhs.size();
                                                    } );

    auto basisIt                = detail::makeTensorProductBasis(basisValues);
    cie::utils::resize( outputContainer, numberOfBasisValues );
    for (auto outputIt=outputContainer.begin(); outputIt!=outputContainer.end(); ++outputIt,++basisIt)
        *outputIt = basisIt.product();
}



template <class BasisType>
inline void
AbsElement<BasisType>::basisDerivatives(   const typename AbsElement::basis_value_container& basisValues,
                                            const typename AbsElement::basis_value_container& basisDerivativeValues,
                                            typename AbsElement::point_container& outputContainer ) const
{
    Size numberOfBasisValues    = std::accumulate(  basisValues.begin(),
                                                    basisValues.end(),
                                                    Size(1),
                                                    [](auto lhs, const auto& rhs) -> Size
                                                    {
                                                        return lhs * rhs.size();
                                                    } );
    auto basisDerivativeIt = detail::makeTensorProductDerivatives(basisValues, basisDerivativeValues);

    cie::utils::resize( outputContainer, numberOfBasisValues );
    for (Size basisIndex=0; basisIndex<numberOfBasisValues; ++basisIndex,++basisDerivativeIt)
        outputContainer[basisIndex]    = basisDerivativeIt.product();
}


// ---------------------------------------------------------
// COMPUTE FIELD VALUES
// ---------------------------------------------------------

template <class BasisType>
template <class CoefficientContainer, concepts::STLContainer BasisContainer>
inline typename AbsElement<BasisType>::NT
AbsElement<BasisType>::operator()(  const CoefficientContainer& coefficients,
                                    const BasisContainer& basisValues ) const
requires concepts::ClassContainer<CoefficientContainer,NT>
            && concepts::ClassContainer<typename BasisContainer::value_type,NT>
{
    // Get basis values
    typename AbsElement::value_container basis;
    this->basis( basisValues, basis );

    CIE_OUT_OF_RANGE_CHECK( coefficients.size() == basis.size() )

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
        for (const auto& function : this->_basisSet.functions(dim))
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
                                    const basis_value_container& basisValues,
                                    const basis_value_container& derivativeValues,
                                    point_type& gradient )
{
    CIE_THROW( AbstractCallException, "Field derivative not implemented" )
}


template <class BasisType>
inline void
AbsElement<BasisType>::derivative( const coefficient_container& coefficients,
                                    const basis_value_container& basisValues,
                                    const basis_value_container& derivativeValues,
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
    auto                                    _derivatives = _basisSet.derivatives();
    point_type                              gradient;

    auto valueIt    = basisValues.begin();
    for (Size dim=0; dim<dimension; ++dim,++valueIt)
        for (const auto& function : this->_basisSet.functions(dim))
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
    auto _derivatives = _basisSet.derivatives();

    for ( ; pointBegin!=pointEnd; ++pointBegin )
    {
        std::array<std::vector<NT>,dimension>   basisValues, derivativeValues;

        auto valueIt    = basisValues.begin();
        for (Size dim=0; dim<dimension; ++dim,++valueIt)
            for (const auto& function : this->_basisSet.functions(dim))
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
    utils::resize(localPoint,dimension);
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
inline typename AbsElement<BasisType>::LocalCoordinates
AbsElement<BasisType>::localCoordinates(  const typename AbsElement<BasisType>::point_type& point ) const
{
    AbsElement<BasisType>::LocalCoordinates output;
    utils::resize(output,dimension);
    this->toLocalCoordinates( point, output );
    return output;
}





// ---------------------------------------------------------
// 1D BASE SPECIALIZATION
// ---------------------------------------------------------

template <class BasisType>
template <class ...Args>
AbsElement1D<BasisType>::AbsElement1D(  const std::pair<typename AbsElement1D<BasisType>::NT, typename AbsElement1D<BasisType>::NT>& domain,
                                        Args&&... args ) :
    AbsElement<BasisType>( std::forward<Args>(args)... ),
    _domain(domain),
    _jacobian( (domain.second-domain.first) / (this->_basisSet.domain(0,1)-this->_basisSet.domain(0,0)) ),
    _invJacobian( 1.0/_jacobian )
{
    CIE_STATIC_ASSERT( BasisType::dimension == 1 )
}


template <class BasisType>
inline void
AbsElement1D<BasisType>::toLocalCoordinates(    const typename AbsElement1D<BasisType>::point_type& point,
                                                typename AbsElement1D<BasisType>::LocalCoordinates& localPoint ) const
{
    CIE_ASSERT(
        (_domain.first <= point[0]) && (point[0] <= _domain.second),
        "Input coordinate " + std::to_string(point[0]) + " outside element domain ["
        + std::to_string(_domain.first) + "," + std::to_string(_domain.second) + "]"
    );
    localPoint[0] = (point[0] - _domain.first) * _invJacobian - this->_basisSet.domain(0,1);
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
    return (coordinate - _domain.first) * _invJacobian - this->_basisSet->domain(0,1);
}


template <class BasisType>
void 
AbsElement1D<BasisType>::_derivative(   const typename AbsElement1D<BasisType>::coefficient_container& coefficients,
                                        const typename AbsElement1D<BasisType>::basis_value_container& basisValues,
                                        const typename AbsElement1D<BasisType>::basis_value_container& basisDerivativeValues,
                                        typename AbsElement1D<BasisType>::point_type& gradient )
{
    std::fill(  gradient.begin(),
                gradient.end(),
                0.0 );
    auto basisDerivatives   = detail::makeTensorProductDerivatives(basisValues, basisDerivativeValues);
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