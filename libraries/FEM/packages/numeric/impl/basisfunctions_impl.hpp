#ifndef CIE_FEM_BASIS_FUNCTIONS_IMPL_HPP
#define CIE_FEM_BASIS_FUNCTIONS_IMPL_HPP

// --- Utility Includes ---
#include <cieutils/concepts.hpp>

namespace cie::fem
{


// ---------------------------------------------------------
// BASE CLASS
// ---------------------------------------------------------

template <  Size Dimension,
            concepts::NumericType NT>
AbsBasisFunctionSet<Dimension,NT>::AbsBasisFunctionSet( const typename AbsBasisFunctionSet<Dimension,NT>::basis_function_container& functions ) :
    _derivatives(nullptr),
    _functions(functions),
    _cache()
{
    utils.setContainerSize( this->_domain, this->dimension );
    for (auto& pair : this->_domain)
    {
        pair.first  = NT(-1.0);
        pair.second = NT(1.0);
    }
}


template <  Size Dimension,
            concepts::NumericType NT>
inline NT 
AbsBasisFunctionSet<Dimension,NT>::operator()(  Size dimension,
                                                Size functionIndex, 
                                                NT coordinate )
{
    if ( _domain[dimension].first <= coordinate && coordinate <= _domain[dimension].second )
        return _functions[dimension][functionIndex](coordinate);
    else
        return NT(0.0);
}


template <  Size Dimension,
            concepts::NumericType NT>
template <class CoordinateIterator, class OutputIterator>
inline void
AbsBasisFunctionSet<Dimension,NT>::operator()(  Size dimension,
                                                Size functionIndex,
                                                CoordinateIterator inputBegin,
                                                CoordinateIterator inputEnd,
                                                OutputIterator outputBegin )
requires concepts::ClassIterator<CoordinateIterator,NT>
            && concepts::ClassIterator<OutputIterator,NT>
{
    for ( ; inputBegin!=inputEnd; inputBegin++)
        *outputBegin++ = this->operator()(dimension,functionIndex,*inputBegin);
}


template <  Size Dimension,
            concepts::NumericType NT>
template <class CoordinateContainer, class OutputContainer>
inline void
AbsBasisFunctionSet<Dimension,NT>::operator()(  Size dimension,
                                                Size functionIndex,
                                                CoordinateContainer coordinates,
                                                OutputContainer outputBegin )
requires concepts::ClassContainer<CoordinateContainer,NT>
                && concepts::ClassContainer<OutputContainer,NT>
{
    this->operator()(   dimension,
                        functionIndex,
                        coordinates.begin(),
                        coordinates.end(),
                        outputBegin );
}


template <  Size Dimension,
            concepts::NumericType NT>
template <template <class ...> class ContainerType, class ValueType, class ...Args>
inline ContainerType<ValueType,Args...>
AbsBasisFunctionSet<Dimension,NT>::operator()(  Size dimension,
                                                Size functionIndex,
                                                const ContainerType<ValueType,Args...>& coordinates )
requires std::is_same_v<ValueType,NT>
{
    ContainerType<ValueType,Args...> output;
    utils::setContinerSize(output,coordinates.size());  // compatible with both std::array and dynamic containers
    this->operator()(   dimension,
                        functionIndex,
                        coordinates.begin(),
                        coordinates.end(),
                        output.begin() );
    return output;
}


// ---------------------------------------------------------
// POLYNOMIAL SET OF BASIS FUNCTIONS
// ---------------------------------------------------------
namespace detail{
template <class ContainerType>
NT evaluatePolynomial( const ContainerType& coefficients, const NT& coordinate )
requires concepts::ClassContainer<ContainerType,NT>
{
    NT power = 1.0;
    NT value = 0.0;
    for (const auto& coefficient : coefficients)
    {
        value += coefficient*power;
        power *= coordinate;
    }
    return value;
}
}


template <  Size Dimension,
            concepts::NumericType NT >
PolynomialBasisFunctionSet<Dimension,NT>::PolynomialBasisFunctionSet( const typename PolynomialBasisFunctionSet::coefficient_container& coefficients ) :
    AbsBasisFunctionSet<Dimension,NT>(),
    _coefficients(coefficients)
{
    // Fill functions
    for (auto& polynomials : coefficients)
    {
        this->_functions.resize(this->_functions.size()+1);
        auto& functions = this->_functions.back();
        for (auto& polynomial : polynomials)
        {
            auto function = [polynomial]( const NT& coordinate ) -> NT
            { return detail::evaluatePolynomial(polynomial,coordinate); };
            functions.push_back(function);
        } // for polynomial in polynomials
    } // for polynomials in dimensions
} // constructor


template <  Size Dimension,
            concepts::NumericType NT >
inline const typename PolynomialBasisFunctionSet<Dimension,NT>::coefficient_container&
PolynomialBasisFunctionSet<Dimension,NT>::coefficients() const
{
    return this->_coefficients;
}


template <  Size Dimension,
            concepts::NumericType NT >
inline const typename PolynomialBasisFunctionSet<Dimension,NT>::polynomial_set&
PolynomialBasisFunctionSet<Dimension,NT>::coefficients( Size dimensionIndex ) const
{
    return this->coefficients[dimensionIndex];
}


template <  Size Dimension,
            concepts::NumericType NT >
inline const typename PolynomialBasisFunctionSet<Dimension,NT>::polynomial_coefficients&
PolynomialBasisFunctionSet<Dimension,NT>::coefficients( Size dimensionIndex, Size polynomialIndex ) const
{
    return this->_coefficients[dimensionIndex][polynomialIndex];
}


template <  Size Dimension,
            concepts::NumericType NT >
inline NT
PolynomialBasisFunctionSet<Dimension,NT>::coefficient(  Size dimensionIndex,
                                                        Size polynomialIndex,
                                                        Size coefficientIndex )
{
    return this->_coefficients[dimensionIndex][polynomialIndex][coefficientIndex];
}


template <  Size Dimension,
            concepts::NumericType NT >
inline void
PolynomialBasisFunctionSet<Dimension,NT>::computeDerivatives()
{
    typename PolynomialBasisFunctionSet<Dimension,NT>::coefficient_container derivativeCoefficients;
    utils::setContainerSize( derivativeCoefficients, this->dimension );
    
    for (Size dim=0; dim<this->dimension; ++dim)
    {
        const auto& polynomials = this->_coefficients[dim];
        auto& derivatives = derivativeCoefficients[dim];
        for (const auto& polynomial : polynomials)
        {
            derivatives.resize(derivatives.size()+1);
            auto& polynomial = derivatives.back();
            if (polynomial.size() < 2)
                polynomial.emplace_back(0.0);
            else
            {

            }
        } // for polynomial in polynomials
    } // for polynomials in dimensions
} // PolynomialBasisFunctionSet::computeDerivatives


}

#endif