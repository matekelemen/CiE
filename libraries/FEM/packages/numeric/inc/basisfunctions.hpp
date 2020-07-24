#ifndef CIE_FEM_BASIS_FUNCTIONS_HPP
#define CIE_FEM_BASIS_FUNCTIONS_HPP

// --- Utility Includes ---
#include <cieutils/concepts.hpp>
#include <cieutils/macros.hpp>

// --- Internal Includes ---
#include "../../utilities/inc/kernel.hpp"

// --- STL Includes ---
#include <functional>
#include <array>
#include <utility>
#include <memory>


namespace cie::fem
{


template <concepts::NumericType NT>                 // NT: number type
using BasisFunction = std::function<NT(const NT&)>;


template <  Size Dimension,
            concepts::NumericType NT>
class AbsBasisFunctionSet : Kernel<NT>
{
public:
    static const Size                               dimension = Dimension;
    typedef Kernel<NT>                              kernel_type;
    typedef std::array<std::pair<NT,NT>,dimension>  domain_container;
    typedef std::array<BasisFunction<NT>,dimension> basis_function_container;

public:
    AbsBasisFunctionSet(    const basis_function_container& basisFunctions,
                            const domain_container& domain );

    const domain_container& domain()                                        { return _domain; }
    const typename domain_container::value_type& domain( Size index )       { return _domain[index]; }
    NT domain( Size index, bool minmax )                                    { return !minmax ? _domain[index].first : _domain[index].second; }
    NT domain( Size index, Size minmax )                                    { return minmax==0 ? _domain[index].first : _domain[index].second; }
    std::unique_ptr<AbsBasisFunctionSet<dimension,NT>>& derivatives()       { return _derivatives; }
    const BasisFunction<NT>& derivative( Size index )                       { CIE_CHECK_POINTER(_derivatives) return _derivatives->operator[](index); }


protected:
    const domain_container                              _domain;
    std::unique_ptr<AbsBasisFunctionSet<dimension,NT>>  _derivatives;
    const basis_function_container                      _basisFunctions;
};


}


#endif