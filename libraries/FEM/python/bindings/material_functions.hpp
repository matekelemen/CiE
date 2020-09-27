#ifndef CIE_FEM_PYTHON_BINDINGS_MATERIAL_FUNCTIONS_HPP
#define CIE_FEM_PYTHON_BINDINGS_MATERIAL_FUNCTIONS_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/basic_concepts.hpp"

// --- Internal Includes ---
#include "FEM/packages/utilities/inc/functional.hpp"


namespace cie::fem {


template <concepts::NumericType NT>
utils::ScalarMap<NT>
makeConstantFunction( NT value )
{
    return utils::ScalarMap<NT>([value]( NT input ) -> NT { return value; });
}



} // namespace cie::fem


#endif