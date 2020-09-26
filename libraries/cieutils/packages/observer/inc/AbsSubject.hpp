#ifndef CIEUTILS_ABSSUBJECT_HPP
#define CIEUTILS_ABSSUBJECT_HPP

// --- Internal Includes ---
#include "cieutils/packages/observer/inc/AbsObserver.hpp"

// --- STL Includes ---
#include <memory>
#include <vector>


namespace cie::utils {


class AbsSubject : public std::enable_shared_from_this<AbsSubject>
{
public:
    AbsSubject() = default;
    AbsSubject( const AbsSubjectPtr& copy );
    AbsSubjectPtr operator=( const AbsSubjectPtr& copy );
    ~AbsSubject();

    void notifyObservers();
    int attachObserver( AbsObserverPtr observer );
    void detachObserver( AbsObserverPtr observer );
    int observerID( AbsObserverPtr observer );

    const std::vector<AbsObserverPtr>& observers() const;

    template <class DerivedSubject>
    std::shared_ptr<DerivedSubject> as();

protected:
    std::vector<AbsObserverPtr>     _observers;
};


} // namespace cie::utils

#include "cieutils/packages/observer/impl/AbsSubject_impl.hpp"

#endif