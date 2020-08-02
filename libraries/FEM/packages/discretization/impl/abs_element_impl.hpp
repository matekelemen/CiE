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
// COMPUTE FIELD VALUES
// ---------------------------------------------------------

template <class BasisType>
template <class CoefficientContainer, class BasisContainer>
inline typename AbsElement<BasisType>::NT
AbsElement<BasisType>::operator()(  const CoefficientContainer& coefficients,
                                    const std::array<BasisContainer,dimension>& basisValues ) const
requires concepts::ClassContainer<CoefficientContainer,NT>
            && concepts::ClassContainer<BasisContainer,NT>
{
    CIE_OUT_OF_RANGE_ASSERT(
        coefficients.size()
        ==
        std::accumulate(    basisValues.begin(),
                            basisValues.end(),
                            Size(1),
                            [](auto lhs, const auto& rhs) -> Size
                            {
                                return lhs * rhs.size();
                            } )
        ,
        "AbsElement::operator()"
    )

    NT value(0.0);

    auto basis = detail::makeTensorProductBasis(basisValues);
    for (const auto& coefficient : coefficients)
    {
        value += (*basis) * coefficient;
        ++basis;
    }

    return value;
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
                                    const std::array<basis_value_container,dimension>& basisValues,
                                    const std::array<basis_value_container,dimension>& derivativeValues,
                                    point_type& gradient )
{
    throw AbstractCallException( "AbsElement::derivative" );
}


template <class BasisType>
inline void
AbsElement<BasisType>::derivative( const coefficient_container& coefficients,
                                    const std::array<basis_value_container,dimension>& basisValues,
                                    const std::array<basis_value_container,dimension>& derivativeValues,
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


}

#endif