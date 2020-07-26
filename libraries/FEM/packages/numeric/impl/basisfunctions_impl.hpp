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
AbsBasisFunctionSet<Dimension,NT>::AbsBasisFunctionSet( const typename AbsBasisFunctionSet<Dimension,NT>::function_container& functions ) :
    _derivatives(nullptr),
    _functions(functions),
    _cache()
{
}


template <  Size Dimension,
            concepts::NumericType NT>
AbsBasisFunctionSet<Dimension,NT>::AbsBasisFunctionSet() :
    AbsBasisFunctionSet<Dimension,NT>::AbsBasisFunctionSet( typename AbsBasisFunctionSet<Dimension,NT>::function_container() )
{
}


template <  Size Dimension,
            concepts::NumericType NT>
inline NT 
AbsBasisFunctionSet<Dimension,NT>::operator()(  Size dimension,
                                                Size functionIndex, 
                                                NT coordinate )
{
    return _functions[dimension][functionIndex](coordinate);
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
                                                const CoordinateContainer& coordinates,
                                                OutputContainer& outputContainer )
requires concepts::ClassContainer<CoordinateContainer,NT>
                && concepts::ClassContainer<OutputContainer,NT>
{
    utils::setContainerSize(outputContainer,coordinates.size());
    this->operator()(   dimension,
                        functionIndex,
                        coordinates.begin(),
                        coordinates.end(),
                        outputContainer.begin() );
}


template <  Size Dimension,
            concepts::NumericType NT>
template <class ContainerType>
inline ContainerType
AbsBasisFunctionSet<Dimension,NT>::operator()(  Size dimension,
                                                Size functionIndex,
                                                const ContainerType& coordinates )
requires concepts::ClassContainer<ContainerType,NT>
{
    ContainerType output;
    utils::setContainerSize(output,coordinates.size());  // compatible with both std::array and dynamic containers
    this->operator()(   dimension,
                        functionIndex,
                        coordinates.begin(),
                        coordinates.end(),
                        output.begin() );
    return output;
}


template <  Size Dimension,
            concepts::NumericType NT>
inline const typename AbsBasisFunctionSet<Dimension,NT>::domain_container
AbsBasisFunctionSet<Dimension,NT>::domain() const
{
    typename AbsBasisFunctionSet<Dimension,NT>::domain_container output;
    utils::setContainerSize(output, this->dimension);
    for (Size dim=0; dim<this->dimension; ++dim)
    {
        assert( !this->_functions[dim].empty() );
        output[dim] = std::make_pair(this->_functions[dim][0]._min, this->_functions[dim][0]._max );
    }
    return output;
}


// ---------------------------------------------------------
// POLYNOMIAL SET OF BASIS FUNCTIONS
// ---------------------------------------------------------
namespace detail{
template <template <class ...> class ContainerType, class NT, class ...Args>
NT evaluatePolynomial( const ContainerType<NT,Args...>& coefficients, const NT& coordinate )
requires concepts::NumericType<NT>
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
    auto coefficientIt  = coefficients.begin();
    auto functionIt     = this->_functions.begin();
    for ( ; coefficientIt!=coefficients.end(); ++coefficientIt,++functionIt )
    {
        for (auto& polynomial : *coefficientIt)
        {
            auto function = [polynomial]( const NT& coordinate ) -> NT
            { return detail::evaluatePolynomial(polynomial,coordinate); };
            functionIt->push_back( typename PolynomialBasisFunctionSet<Dimension,NT>::function_type(function) );
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
                                                        Size coefficientIndex ) const
{
    return this->_coefficients[dimensionIndex][polynomialIndex][coefficientIndex];
}


template <  Size Dimension,
            concepts::NumericType NT >
inline Size
PolynomialBasisFunctionSet<Dimension,NT>::polynomialDegree( Size dimensionIndex, Size polynomialIndex ) const
{
    return this->_coefficients[dimensionIndex][polynomialIndex].size() - 1;
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
            auto& derivative = derivatives.back();
            if (polynomial.size() < 2)
                derivative.emplace_back(0.0);
            else
            {
                for (Size power=1; power<polynomial.size(); ++power)
                    derivative.emplace_back( NT(power)*polynomial[power] );
            }
        } // for polynomial in polynomials
    } // for polynomials in dimensions

    this->_derivatives = std::make_shared<PolynomialBasisFunctionSet<Dimension,NT>>(
        derivativeCoefficients
    );
} // PolynomialBasisFunctionSet::computeDerivatives


}

#endif