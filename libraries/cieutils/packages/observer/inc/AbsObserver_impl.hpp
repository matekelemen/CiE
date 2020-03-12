#ifndef CIEUTILS_ABSOBSERVER_IMPL_HPP
#define CIEUTILS_ABSOBSERVER_IMPL_HPP

namespace cie {
namespace utils {


template <class DerivedObject>
std::shared_ptr<DerivedObject> AbsObserver::as()
{
    return std::dynamic_pointer_cast<DerivedObject>(this);
}


}
}

#endif