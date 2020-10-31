#ifndef CIEUTILS_ABSOBSERVER_HPP
#define CIEUTILS_ABSOBSERVER_HPP

// --- STL Imports ---
#include <memory>


namespace cie::utils {

class AbsObserver;
class AbsSubject;
using AbsObserverPtr    = std::shared_ptr<AbsObserver>;
using AbsSubjectPtr     = std::shared_ptr<AbsSubject>;


class AbsObserver : public std::enable_shared_from_this<AbsObserver>
{
public:
    AbsObserver();
    AbsObserver( AbsSubjectPtr subject );
    AbsObserver( const AbsObserverPtr& copy );
    AbsObserverPtr operator=( const AbsObserverPtr& copy );
    ~AbsObserver();

    virtual void onSubjectChange() = 0;
    void detach();

    template <class DerivedObserver>
    std::shared_ptr<DerivedObserver> as();

    void setSubject( AbsSubjectPtr subject );
    AbsSubjectPtr subject();
    const AbsSubjectPtr subject() const;

protected:
    std::weak_ptr<AbsSubject>   _subject;
};


} // namespace cie::utils

#include "cieutils/packages/observer/impl/AbsObserver_impl.hpp"

#endif