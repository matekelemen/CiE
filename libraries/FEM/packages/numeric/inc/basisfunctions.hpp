#ifndef CIE_FEM_BASIS_FUNCTIONS_HPP
#define CIE_FEM_BASIS_FUNCTIONS_HPP

// --- Utility Includes ---
#include <cieutils/cache.hpp>
#include <cieutils/concepts.hpp>
#include <cieutils/macros.hpp>

// --- Internal Includes ---
#include "../../utilities/inc/kernel.hpp"

// --- STL Includes ---
#include <functional>
#include <array>
#include <vector>
#include <utility>
#include <memory>


namespace cie::fem
{


template <concepts::NumericType NT>                 // NT: number type
using BasisFunction = std::function<NT(const NT&)>;


// ---------------------------------------------------------
// BASE CLASS
// ---------------------------------------------------------
template <  Size Dimension,
            concepts::NumericType NT>
class AbsBasisFunctionSet : Kernel<NT>
{
public:                                                         // <-- member typedefs
    static const Size                                           dimension = Dimension;
    typedef KernelType                                          kernel_type;
    typedef std::array<std::pair<NT,NT>,dimension>              domain_container;
    typedef BasisFunction<NT>                                   function_type;
    typedef std::array<std::vector<function_type>,dimension>    function_container;
    typedef utils::Cache<std::vector<NT>,std::vector<NT>>       cache_type;

public:                                                         // <-- main functions 
    AbsBasisFunctionSet( const basis_function_container& functions );
    AbsBasisFunctionSet() : _derivatives(nullptr)               {}

    NT operator()(  Size dimension,
                    Size functionIndex, 
                    NT coordinate );

    template <class CoordinateIterator, class OutputIterator>
    void operator()(    Size dimension,
                        Size functionIndex,
                        CoordinateIterator inputBegin,
                        CoordinateIterator inputEnd,
                        OutputIterator outputBegin )
    requires concepts::ClassIterator<CoordinateIterator,NT>
                && concepts::ClassIterator<OutputIterator,NT>;

    template <class CoordinateContainer, class OutputContainer>
    void operator()(    Size dimension,
                        Size functionIndex,
                        CoordinateContainer coordinates,
                        OutputContainer outputBegin )
    requires concepts::ClassContainer<CoordinateContainer,NT>
                && concepts::ClassContainer<OutputContainer,NT>;

    template <template <class ...> class ContainerType, class ValueType, class ...Args>
    ContainerType<ValueType,Args...> operator()(    Size dimension,
                                                    Size functionIndex,
                                                    const ContainerType<ValueType,Args...>& coordinates )
    requires std::is_same_v<ValueType,NT>;

public:                                                                         // <-- Set/get
    const domain_container& domain()                                            { return _domain; }
    const typename domain_container::value_type& domain( Size index )           { return _domain[index]; }
    NT domain( Size index, bool minmax )                                        { return !minmax ? _domain[index].first : _domain[index].second; }
    NT domain( Size index, Size minmax )                                        { return minmax==0 ? _domain[index].first : _domain[index].second; }
    virtual std::unique_ptr<AbsBasisFunctionSet<dimension,NT>>& derivatives()   { return _derivatives; }
    virtual const function_type& derivative( Size index )                       { CIE_CHECK_POINTER(_derivatives) return _derivatives->operator[](index); }
    const function_container& functions()                                       { return _functions; }
    function_type function( Size index )                                        { return _functions[index]; }
    const function_type& operator[]( Size index )                               { return _functions[index]; }

protected:                                                                      // <-- member variables
    std::unique_ptr<AbsBasisFunctionSet<dimension,NT>>  _derivatives;
    cache_type                                          _cache;
    function_container                                  _functions;
    domain_container                                    _domain;
};



// ---------------------------------------------------------
// POLYNOMIAL SET OF BASIS FUNCTIONS
// ---------------------------------------------------------
namespace detail{
template <class ContainerType>
NT evaluatePolynomial( const ContainerType& coefficients, const NT& coordinate )
requires concepts::ClassContainer<ContainerType,NT>;
}


template <  Size Dimension,
            concepts::NumericType NT >
class PolynomialBasisFunctionSet : public AbsBasisFunctionSet<Dimension,NT>
{
public:
    typedef std::vector<NT>                                                     polynomial_coefficients;
    typedef std::vector<polynomial_coefficients>                                polynomial_set;
    typedef std::array<polynomial_set,PolynomialBasisFunctionSet::dimension>    coefficient_container;

public:
    PolynomialBasisFunctionSet( const coefficient_container& coefficients );
    PolynomialBasisFunctionSet() = default;

    const coefficient_container& coefficients() const;
    const polynomial_set& coefficients( Size dimensionIndex ) const;
    const polynomial_coefficients& coefficients( Size dimensionIndex, Size polynomialIndex ) const;
    const NT coefficient( Size dimensionIndex, Size polynomialIndex, Size coefficientIndex ) const;

protected:
    void computeDerivatives();
    
protected:
    coefficient_container   _coefficients;
};


}

#include "../impl/basisfunctions_impl.hpp"

#endif