#ifndef CIEUTILS_ABSSUBJECT_IMPL_HPP
#define CIEUTILS_ABSSUBJECT_IMPL_HPP

namespace cie::utils{


template <class DerivedSubject>
std::shared_ptr<DerivedSubject> AbsSubject::as()
{
    return std::dynamic_pointer_cast<DerivedSubject>( shared_from_this() );
}


} // namespace cie::utils

#endif