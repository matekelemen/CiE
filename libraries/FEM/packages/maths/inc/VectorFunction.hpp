#ifndef CIE_FEM_MATHS_VECTOR_FUNCTION_HPP
#define CIE_FEM_MATHS_VECTOR_FUNCTION_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/basic_concepts.hpp"
#include "cieutils/packages/types/inc/types.hpp"

// --- Internal Includes ---
#include "FEM/packages/maths/inc/MatrixFunction.hpp"
#include "FEM/packages/maths/inc/AbsFunction.hpp"
#include "FEM/packages/utilities/inc/kernel.hpp"


namespace cie::fem::maths {


template < Size ValueDimension,
           Size Dimension,
           concepts::NumericType NT,
           class DerivativeType = MatrixFunction<ValueDimension,Dimension,Dimension,NT> >
struct VectorFunction :
    public AbsFunction< typename Kernel<ValueDimension,NT>::point_type,
                        typename Kernel<Dimension,NT>::point_type,
                        DerivativeType >,
    public Kernel<Dimension,NT>
{
public:
    CIE_DEFINE_CLASS_POINTERS( VectorFunction )
};


template < Size VD,
           Size D,
           concepts::NumericType NT >
using VectorFunctionPtr = std::shared_ptr<VectorFunction<VD,D,NT>>;


} // namespace cie::fem::maths


#endif