#ifndef CIE_FEM_MATHS_OPERATION_HPP
#define CIE_FEM_MATHS_OPERATION_HPP

// --- Internal Includes ---
#include "FEM/packages/maths/inc/AbsFunction.hpp"


namespace cie::fem::maths {


template <class ResultType>
class Operation : public ResultType
{
public:
    CIE_DEFINE_CLASS_POINTERS( Operation )

public:
    template <class ...Arguments>
    requires concepts::AbsFunction<ResultType>
    Operation( Arguments&&... r_arguments )
        : ResultType( std::forward<Arguments>(r_arguments)... )
    {}
};


} // namespace cie::fem::maths


#endif