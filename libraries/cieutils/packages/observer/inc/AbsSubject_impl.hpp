#ifndef CIEUTILS_ABSSUBJECT_IMPL_HPP
#define CIEUTILS_ABSSUBJECT_IMPL_HPP

namespace cie {
namespace utils{


template <class DerivedSubject>
std::shared_ptr<DerivedSubject> AbsSubject::as()
{
    return std::dynamic_pointer_cast<DerivedSubject>(this);
}


}
}

#endif