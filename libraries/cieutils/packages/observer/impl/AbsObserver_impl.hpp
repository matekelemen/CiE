#ifndef CIEUTILS_ABSOBSERVER_IMPL_HPP
#define CIEUTILS_ABSOBSERVER_IMPL_HPP

namespace cie::utils {


template <class DerivedObserver>
std::shared_ptr<DerivedObserver> AbsObserver::as()
{
    return std::dynamic_pointer_cast<DerivedObserver>( shared_from_this() );
}


} // namespace cie::utils

#endif