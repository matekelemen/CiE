#ifndef CIE_FEM_ANSATZ_FUNCTIONS_IMPL_HPP
#define CIE_FEM_ANSATZ_FUNCTIONS_IMPL_HPP

// --- Utility Includes ---
#include <cieutils/concepts.hpp>

namespace cie::fem
{


// ---------------------------------------------------------
// BASE CLASS
// ---------------------------------------------------------

template <  Size Dimension,
            concepts::NumericType NT,
            class SelfType >
AbsAnsatzFunctionSet<Dimension,NT,SelfType>::AbsAnsatzFunctionSet( const typename AbsAnsatzFunctionSet<Dimension,NT,SelfType>::function_container& functions ) :
    _derivatives(nullptr),
    _functions(functions)
{
}


template <  Size Dimension,
            concepts::NumericType NT,
            class SelfType >
AbsAnsatzFunctionSet<Dimension,NT,SelfType>::AbsAnsatzFunctionSet() :
    AbsAnsatzFunctionSet<Dimension,NT,SelfType>::AbsAnsatzFunctionSet( typename AbsAnsatzFunctionSet<Dimension,NT,SelfType>::function_container() )
{
}


template <  Size Dimension,
            concepts::NumericType NT,
            class SelfType >
inline NT 
AbsAnsatzFunctionSet<Dimension,NT,SelfType>::operator()( Size dimension,
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
AbsAnsatzFunctionSet<Dimension,NT,SelfType>::operator()( Size dimension,
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
AbsAnsatzFunctionSet<Dimension,NT,SelfType>::operator()( Size dimension,
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
            concepts::NumericType NT,
            class SelfType >
template <class ContainerType>
inline ContainerType
AbsAnsatzFunctionSet<Dimension,NT,SelfType>::operator()( Size dimension,
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
            concepts::NumericType NT,
            class SelfType >
inline const typename AbsAnsatzFunctionSet<Dimension,NT,SelfType>::domain_container
AbsAnsatzFunctionSet<Dimension,NT,SelfType>::domain() const
{
    typename AbsAnsatzFunctionSet<Dimension,NT,SelfType>::domain_container output;
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
            concepts::NumericType NT,
            class SelfType >
AbsPolynomialAnsatzFunctionSet<Dimension,NT,SelfType>::AbsPolynomialAnsatzFunctionSet( const typename AbsPolynomialAnsatzFunctionSet::coefficient_container& coefficients ) :
    AbsAnsatzFunctionSet<Dimension,NT,SelfType>(),
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
            functionIt->push_back( typename AbsPolynomialAnsatzFunctionSet<Dimension,NT,SelfType>::function_type(function) );
        } // for polynomial in polynomials
    } // for polynomials in dimensions
} // constructor


template <  Size Dimension,
            concepts::NumericType NT,
            class SelfType >
inline const typename AbsPolynomialAnsatzFunctionSet<Dimension,NT,SelfType>::coefficient_container&
AbsPolynomialAnsatzFunctionSet<Dimension,NT,SelfType>::coefficients() const
{
    return this->_coefficients;
}


template <  Size Dimension,
            concepts::NumericType NT,
            class SelfType >
inline const typename AbsPolynomialAnsatzFunctionSet<Dimension,NT,SelfType>::polynomial_set&
AbsPolynomialAnsatzFunctionSet<Dimension,NT,SelfType>::coefficients( Size dimensionIndex ) const
{
    return this->coefficients[dimensionIndex];
}


template <  Size Dimension,
            concepts::NumericType NT,
            class SelfType >
inline const typename AbsPolynomialAnsatzFunctionSet<Dimension,NT,SelfType>::polynomial_coefficients&
AbsPolynomialAnsatzFunctionSet<Dimension,NT,SelfType>::coefficients( Size dimensionIndex, Size polynomialIndex ) const
{
    return this->_coefficients[dimensionIndex][polynomialIndex];
}


template <  Size Dimension,
            concepts::NumericType NT,
            class SelfType >
inline NT
AbsPolynomialAnsatzFunctionSet<Dimension,NT,SelfType>::coefficient(  Size dimensionIndex,
                                                        Size polynomialIndex,
                                                        Size coefficientIndex ) const
{
    return this->_coefficients[dimensionIndex][polynomialIndex][coefficientIndex];
}


template <  Size Dimension,
            concepts::NumericType NT,
            class SelfType >
inline Size
AbsPolynomialAnsatzFunctionSet<Dimension,NT,SelfType>::polynomialDegree( Size dimensionIndex, Size polynomialIndex ) const
{
    return this->_coefficients[dimensionIndex][polynomialIndex].size() - 1;
}


template <  Size Dimension,
            concepts::NumericType NT,
            class SelfType >
inline std::shared_ptr<SelfType>&
AbsPolynomialAnsatzFunctionSet<Dimension,NT,SelfType>::derivatives()
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
AbsPolynomialAnsatzFunctionSet<Dimension,NT,SelfType>::computeDerivatives()
{
    typename AbsPolynomialAnsatzFunctionSet<Dimension,NT,SelfType>::coefficient_container derivativeCoefficients;
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

    this->_derivatives = std::make_shared<SelfType>(
        derivativeCoefficients
    );
} // AbsPolynomialAnsatzFunctionSet::computeDerivatives



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
    utils::setContainerSize(coefficients,PolynomialBasis::dimension);
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
AbsLinearAnsatzFunctionSet<Dimension,NT,SelfType>::AbsLinearAnsatzFunctionSet() :
    AbsPolynomialAnsatzFunctionSet<Dimension,NT,SelfType>( detail::linearPolynomialCoefficients(this) )
{
}


}

#endif