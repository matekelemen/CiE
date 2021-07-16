#ifndef CIE_FEM_MATHS_NO_IMPLMENTED_FUNCTION_IMPL_HPP
#define CIE_FEM_MATHS_NO_IMPLMENTED_FUNCTION_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"


namespace cie::fem::maths::detail {


template <class ...Arguments>
AllType::AllType( Arguments&&... r_arguments )
{
    CIE_THROW( NotImplementedException, "attempting to construct a dummy object for NotImplementedFunction" )
}


template <class ...Arguments>
AllType AllType::operator()( Arguments&&... r_arguments ) const
{
    CIE_THROW( NotImplementedException, "attempting to call operator() on the result of NotImplementedFunction" )
    return AllType();
}


template <class ArgumentType>
AllType AllType::operator[]( ArgumentType&& r_argument ) const
{
    CIE_THROW( NotImplementedException, "attempting to call operator[] on the result of NotImplementedFunction" )
    return AllType();
}


template <class T>
AllType::operator T()
{
    CIE_THROW( NotImplementedException, "attempting to convert the result of NotImplementedFunction" )
}



} // namespace cie::fem::maths::detail


#endif