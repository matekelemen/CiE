#ifndef CIE_FEM_ANSATZ_FUNCTIONS_HPP
#define CIE_FEM_ANSATZ_FUNCTIONS_HPP

// --- Utility Includes ---
#include "cieutils/packages/cache/inc/cache.hpp"
#include "cieutils/packages/concepts/inc/iterator_concepts.hpp"

// --- Internal Includes ---
#include "FEM/packages/utilities/inc/kernel.hpp"

// --- STL Includes ---
#include <functional>
#include <array>
#include <vector>
#include <utility>
#include <memory>


namespace cie::fem {


// ---------------------------------------------------------
// ANSATZ FUNCTION
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
            concepts::NumericType NT,
            class SelfType >
class AbsBasisFunctionSet : Kernel<Dimension,NT>
{
public:                                                         // <-- member typedefs
    static const Size                                           dimension = Dimension;
    typedef Kernel<Dimension,NT>                                kernel_type;
    typedef std::array<std::pair<NT,NT>,dimension>              domain_container;
    typedef BasisFunction<NT>                                   function_type;
    typedef std::array<std::vector<function_type>,dimension>    function_container;

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
    const typename domain_container::value_type domain( Size index ) const          { return std::make_pair( _functions[index][0]._min, _functions[index][0]._max ); }
    NT domain( Size index, Size minmax ) const                                      { return minmax==0 ? _functions[index][0]._min : _functions[index][0]._max; }
    virtual std::shared_ptr<SelfType>& derivatives()                                { return _derivatives; }
    const function_container& functions() const                                     { return _functions; }
    const typename function_container::value_type& functions(Size index) const      { return _functions[index]; }
    const typename function_container::value_type& operator[]( Size index ) const   { return _functions[index]; }

protected:                                                                      // <-- member variables
    std::shared_ptr<SelfType>   _derivatives;
    function_container          _functions;
};



// ---------------------------------------------------------
// POLYNOMIAL SET OF ANSATZ FUNCTIONS
// ---------------------------------------------------------

namespace detail{
// Helper function for evaluating polynomials defined by their coefficients
template <template <class ...> class ContainerType, class NT, class ...Args>
NT evaluatePolynomial( const ContainerType<NT,Args...>& coefficients, const NT& coordinate )
requires concepts::NumericType<NT>;
}


template <  Size Dimension,
            concepts::NumericType NT,
            class SelfType >
class AbsPolynomialBasisFunctionSet : public AbsBasisFunctionSet<Dimension,NT,SelfType>
{
public:
    typedef std::vector<NT>                                                     polynomial_coefficients;
    typedef std::vector<polynomial_coefficients>                                polynomial_set;
    typedef std::array<polynomial_set,AbsPolynomialBasisFunctionSet::dimension> coefficient_container;

public:
    AbsPolynomialBasisFunctionSet( const coefficient_container& coefficients );
    AbsPolynomialBasisFunctionSet() = delete;

    Size polynomialDegree( Size dimensionIndex, Size polynomialIndex ) const;
    std::shared_ptr<SelfType>& derivatives() override;

    const coefficient_container& coefficients() const;
    const polynomial_set& coefficients( Size dimensionIndex ) const;
    const polynomial_coefficients& coefficients( Size dimensionIndex, Size polynomialIndex ) const;
    NT coefficient( Size dimensionIndex, Size polynomialIndex, Size coefficientIndex ) const;

protected:
    void computeDerivatives();
    
protected:
    coefficient_container   _coefficients;
}; // class AbsPolynomialBasisFunctionSet




// Self-contained polynomial basis function set
template <  Size Dimension,
            concepts::NumericType NT >
struct PolynomialBasisFunctionSet final 
    : AbsPolynomialBasisFunctionSet<Dimension,NT,PolynomialBasisFunctionSet<Dimension,NT>>
{
    PolynomialBasisFunctionSet( const typename PolynomialBasisFunctionSet::coefficient_container& coefficients ) :
        AbsPolynomialBasisFunctionSet<Dimension,NT,PolynomialBasisFunctionSet<Dimension,NT>>::AbsPolynomialBasisFunctionSet(coefficients)
    {}

    PolynomialBasisFunctionSet() = delete;
}; // class PolynomialBasisFunctionSet



// ---------------------------------------------------------
// LINEAR SET OF ANSATZ FUNCTIONS
// ---------------------------------------------------------
template <  Size Dimension,
            concepts::NumericType NT,
            class SelfType >
struct AbsLinearBasisFunctionSet : AbsPolynomialBasisFunctionSet<Dimension,NT,SelfType>
{
    AbsLinearBasisFunctionSet();
};



template <  Size Dimension,
            concepts::NumericType NT >
struct LinearBasisFunctionSet final : 
    AbsLinearBasisFunctionSet<Dimension,NT,PolynomialBasisFunctionSet<Dimension,NT>>
{
    LinearBasisFunctionSet() :
        AbsLinearBasisFunctionSet<Dimension,NT,PolynomialBasisFunctionSet<Dimension,NT>>()
    {}
};


} // namespace cie::fem

#include "FEM/packages/numeric/impl/basisfunctions_impl.hpp"

#endif