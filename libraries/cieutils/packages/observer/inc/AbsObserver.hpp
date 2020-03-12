#ifndef CIEUTILS_ABSOBSERVER_HPP
#define CIEUTILS_ABSOBSERVER_HPP

// --- STD Imports ---
#include <memory>


namespace cie {
namespace utils {

class AbsObserver;
class AbsSubject;
using AbsObserverPtr    = std::shared_ptr<AbsObserver>;
using AbsSubjectPtr     = std::shared_ptr<AbsSubject>;


class AbsObserver
{
public:
    AbsObserver();
    AbsObserver( AbsSubjectPtr subject );
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


}
}

#include "AbsObserver_impl.hpp"

#endif