#ifndef CIE_FEM_ANSATZ_FUNCTIONS_IMPL_HPP
#define CIE_FEM_ANSATZ_FUNCTIONS_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/container_concepts.hpp"
#include "cieutils/packages/stl_extension/inc/resize.hpp"

namespace cie::fem
{


// ---------------------------------------------------------
// BASE CLASS
// ---------------------------------------------------------

template <  Size Dimension,
            concepts::NumericType NT,
            class SelfType >
AbsBasisFunctionSet<Dimension,NT,SelfType>::AbsBasisFunctionSet( const typename AbsBasisFunctionSet<Dimension,NT,SelfType>::function_container& functions ) :
    _derivatives(nullptr),
    _functions(functions)
{
}


template <  Size Dimension,
            concepts::NumericType NT,
            class SelfType >
AbsBasisFunctionSet<Dimension,NT,SelfType>::AbsBasisFunctionSet() :
    AbsBasisFunctionSet<Dimension,NT,SelfType>::AbsBasisFunctionSet( typename AbsBasisFunctionSet<Dimension,NT,SelfType>::function_container() )
{
}


template <  Size Dimension,
            concepts::NumericType NT,
            class SelfType >
inline NT 
AbsBasisFunctionSet<Dimension,NT,SelfType>::operator()( Size dimension,
                                                        Size functionIndex, 
                                                        NT coordinate )
{
    return _functions[dimension][functionIndex](coordinate);
}


template <  Size Dimension,
            concepts::NumericType NT,
            class SelfType >
template <class CoordinateIterator, class OutputIterator>
inline void
AbsBasisFunctionSet<Dimension,NT,SelfType>::operator()( Size dimension,
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
            concepts::NumericType NT,
            class SelfType >
template <class CoordinateContainer, class OutputContainer>
inline void
AbsBasisFunctionSet<Dimension,NT,SelfType>::operator()( Size dimension,
                                                        Size functionIndex,
                                                        const CoordinateContainer& coordinates,
                                                        OutputContainer& outputContainer )
requires concepts::ClassContainer<CoordinateContainer,NT>
                && concepts::ClassContainer<OutputContainer,NT>
{
    utils::resize(outputContainer,coordinates.size());
    this->operator()(   dimension,
                        functionIndex,
                        coordinates.begin(),
                        coordinates.end(),
                        outputContainer.begin() );
}


template <  Size Dimension,
            concepts::NumericType NT,
            class SelfType >
template <class ContainerType>
inline ContainerType
AbsBasisFunctionSet<Dimension,NT,SelfType>::operator()( Size dimension,
                                                        Size functionIndex,
                                                        const ContainerType& coordinates )
requires concepts::ClassContainer<ContainerType,NT>
{
    ContainerType output;
    utils::resize(output,coordinates.size());  // compatible with both std::array and dynamic containers
    this->operator()(   dimension,
                        functionIndex,
                        coordinates.begin(),
                        coordinates.end(),
                        output.begin() );
    return output;
}


template <  Size Dimension,
            concepts::NumericType NT,
            class SelfType >
inline const typename AbsBasisFunctionSet<Dimension,NT,SelfType>::domain_container
AbsBasisFunctionSet<Dimension,NT,SelfType>::domain() const
{
    typename AbsBasisFunctionSet<Dimension,NT,SelfType>::domain_container output;
    utils::resize(output, this->dimension);
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
            concepts::NumericType NT,
            class SelfType >
AbsPolynomialBasisFunctionSet<Dimension,NT,SelfType>::AbsPolynomialBasisFunctionSet( const typename AbsPolynomialBasisFunctionSet::coefficient_container& coefficients ) :
    AbsBasisFunctionSet<Dimension,NT,SelfType>(),
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
            functionIt->push_back( typename AbsPolynomialBasisFunctionSet<Dimension,NT,SelfType>::function_type(function) );
        } // for polynomial in polynomials
    } // for polynomials in dimensions
} // constructor


template <  Size Dimension,
            concepts::NumericType NT,
            class SelfType >
inline const typename AbsPolynomialBasisFunctionSet<Dimension,NT,SelfType>::coefficient_container&
AbsPolynomialBasisFunctionSet<Dimension,NT,SelfType>::coefficients() const
{
    return this->_coefficients;
}


template <  Size Dimension,
            concepts::NumericType NT,
            class SelfType >
inline const typename AbsPolynomialBasisFunctionSet<Dimension,NT,SelfType>::polynomial_set&
AbsPolynomialBasisFunctionSet<Dimension,NT,SelfType>::coefficients( Size dimensionIndex ) const
{
    return this->coefficients[dimensionIndex];
}


template <  Size Dimension,
            concepts::NumericType NT,
            class SelfType >
inline const typename AbsPolynomialBasisFunctionSet<Dimension,NT,SelfType>::polynomial_coefficients&
AbsPolynomialBasisFunctionSet<Dimension,NT,SelfType>::coefficients( Size dimensionIndex, Size polynomialIndex ) const
{
    return this->_coefficients[dimensionIndex][polynomialIndex];
}


template <  Size Dimension,
            concepts::NumericType NT,
            class SelfType >
inline NT
AbsPolynomialBasisFunctionSet<Dimension,NT,SelfType>::coefficient(  Size dimensionIndex,
                                                        Size polynomialIndex,
                                                        Size coefficientIndex ) const
{
    return this->_coefficients[dimensionIndex][polynomialIndex][coefficientIndex];
}


template <  Size Dimension,
            concepts::NumericType NT,
            class SelfType >
inline Size
AbsPolynomialBasisFunctionSet<Dimension,NT,SelfType>::polynomialDegree( Size dimensionIndex, Size polynomialIndex ) const
{
    return this->_coefficients[dimensionIndex][polynomialIndex].size() - 1;
}


template <  Size Dimension,
            concepts::NumericType NT,
            class SelfType >
inline std::shared_ptr<SelfType>&
AbsPolynomialBasisFunctionSet<Dimension,NT,SelfType>::derivatives()
{
    // Check if derivatives were computed
    if (this->_derivatives != nullptr)
        return this->_derivatives;
    
    // Compute derivatives if they haven't been computed already
    this->computeDerivatives();
    return this->_derivatives;
}


template <  Size Dimension,
            concepts::NumericType NT,
            class SelfType >
inline void
AbsPolynomialBasisFunctionSet<Dimension,NT,SelfType>::computeDerivatives()
{
    typename AbsPolynomialBasisFunctionSet<Dimension,NT,SelfType>::coefficient_container derivativeCoefficients;
    utils::resize( derivativeCoefficients, this->dimension );
    
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

    this->_derivatives = std::make_shared<SelfType>(
        derivativeCoefficients
    );
} // AbsPolynomialBasisFunctionSet::computeDerivatives



// ---------------------------------------------------------
// LINEAR SET OF BASIS FUNCTIONS
// ---------------------------------------------------------
namespace detail {
// Helper function for constructing linear basis functions 
template <class PolynomialBasis>
inline const typename PolynomialBasis::coefficient_container
linearPolynomialCoefficients( PolynomialBasis* instance )
{
    typedef typename PolynomialBasis::kernel_type::number_type NT;

    typename PolynomialBasis::coefficient_container coefficients;
    utils::resize(coefficients,PolynomialBasis::dimension);
    for (auto& coefficientSet : coefficients)
    {
        coefficientSet.emplace_back( std::initializer_list<NT>({ 0.5, 0.5 }) );
        coefficientSet.emplace_back( std::initializer_list<NT>({ 0.5, -0.5 }) );
    }
    return coefficients;
}
}


template <  Size Dimension,
            concepts::NumericType NT,
            class SelfType >
AbsLinearBasisFunctionSet<Dimension,NT,SelfType>::AbsLinearBasisFunctionSet() :
    AbsPolynomialBasisFunctionSet<Dimension,NT,SelfType>( detail::linearPolynomialCoefficients(this) )
{
}


} // namespace cie::fem

#endif