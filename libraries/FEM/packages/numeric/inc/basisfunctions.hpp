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


// ---------------------------------------------------------
// BASIS FUNCTION
// ---------------------------------------------------------
template <concepts::NumericType NT>                 // NT: number type
struct BasisFunction
{
public:
    typedef std::function<NT(const NT&)> function_type;

public:
    BasisFunction(  function_type function,
                    const NT& min,
                    const NT& max ) :
        _function(function),
        _min(min),
        _max(max) {}
    
    BasisFunction( function_type function ) :
        _function(function),
        _min(-1.0),
        _max(1.0) {}

    NT operator()( const NT& coordinate ) const
        { return _min<=coordinate && coordinate<=_max ? _function(coordinate) : NT(0.0); }

public:
    function_type   _function;
    NT              _min;
    NT              _max;
};


// ---------------------------------------------------------
// BASE CLASS
// ---------------------------------------------------------
template <  Size Dimension,
            concepts::NumericType NT>
class AbsBasisFunctionSet : Kernel<NT>
{
public:                                                         // <-- member typedefs
    static const Size                                           dimension = Dimension;
    typedef Kernel<NT>                                          kernel_type;
    typedef std::array<std::pair<NT,NT>,dimension>              domain_container;
    typedef BasisFunction<NT>                                   function_type;
    typedef std::array<std::vector<function_type>,dimension>    function_container;
    typedef utils::Cache<std::vector<NT>,std::vector<NT>>       cache_type;

public:                                                         // <-- main functions 
    AbsBasisFunctionSet( const function_container& functions );
    AbsBasisFunctionSet();

    NT operator()(  Size dimension,
                    Size functionIndex, 
                    NT coordinate );

    template <class CoordinateIterator, class OutputIterator>
    void operator()(    Size dimension,
                        Size functionIndex,
                        CoordinateIterator inputBegin,
                        CoordinateIterator inputEnd,
                        OutputIterator outputBegin )
    requires concepts::ClassIterator<CoordinateIterator,NT>;

    template <class CoordinateContainer, class OutputContainer>
    void operator()(    Size dimension,
                        Size functionIndex,
                        const CoordinateContainer& coordinates,
                        OutputContainer& outputContainer )
    requires concepts::ClassContainer<CoordinateContainer,NT>
                && concepts::ClassContainer<OutputContainer,NT>;

    template <class ContainerType>
    ContainerType operator()(   Size dimension,
                                Size functionIndex,
                                const ContainerType& coordinates )
    requires concepts::ClassContainer<ContainerType,NT>;

public:                                                                         // <-- Set/get
    const domain_container domain() const;
    const typename domain_container::value_type domain( Size index ) const                  { return std::make_pair( _functions[index][0]._min, _functions[index][0]._max ); }
    NT domain( Size index, bool minmax ) const                                              { return !minmax ? _functions[index][0]._min : _functions[index][0]._max; }
    NT domain( Size index, Size minmax ) const                                              { return minmax==0 ? _functions[index][0]._min : _functions[index][0]._max; }
    virtual std::shared_ptr<AbsBasisFunctionSet<dimension,NT>>& derivatives()               { return _derivatives; }
    virtual const std::shared_ptr<AbsBasisFunctionSet<dimension,NT>> derivatives() const    { return _derivatives; }
    const function_container& functions() const                                             { return _functions; }
    const typename function_container::value_type& functions(Size index) const              { return _functions[index]; }
    const typename function_container::value_type& operator[]( Size index ) const           { return _functions[index]; }

protected:                                                                      // <-- member variables
    std::shared_ptr<AbsBasisFunctionSet<dimension,NT>>  _derivatives;
    function_container                                  _functions;
    cache_type                                          _cache;
};



// ---------------------------------------------------------
// POLYNOMIAL SET OF BASIS FUNCTIONS
// ---------------------------------------------------------

namespace detail{
template <template <class ...> class ContainerType, class NT, class ...Args>
NT evaluatePolynomial( const ContainerType<NT,Args...>& coefficients, const NT& coordinate )
requires concepts::NumericType<NT>;
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
    PolynomialBasisFunctionSet() = delete;

    Size polynomialDegree( Size dimensionIndex, Size polynomialIndex ) const;
    //std::shared_ptr<AbsBasisFunctionSet<Dimension,NT>>& derivatives() 
    const typename PolynomialBasisFunctionSet::function_type& derivative( Size index );

    const coefficient_container& coefficients() const;
    const polynomial_set& coefficients( Size dimensionIndex ) const;
    const polynomial_coefficients& coefficients( Size dimensionIndex, Size polynomialIndex ) const;
    NT coefficient( Size dimensionIndex, Size polynomialIndex, Size coefficientIndex ) const;

protected:
    void computeDerivatives();
    
protected:
    coefficient_container   _coefficients;
};


}

#include "../impl/basisfunctions_impl.hpp"

#endif