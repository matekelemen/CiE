#ifndef CIEUTILS_ABSSUBJECT_IMPL_HPP
#define CIEUTILS_ABSSUBJECT_IMPL_HPP

// --- Internal Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"


namespace cie::utils{


template <class DerivedSubject>
std::shared_ptr<DerivedSubject> AbsSubject::as()
{
    CIE_BEGIN_EXCEPTION_TRACING

    return std::dynamic_pointer_cast<DerivedSubject>( shared_from_this() );

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::utils

#endif