#ifndef CIEUTILS_ABSSUBJECT_HPP
#define CIEUTILS_ABSSUBJECT_HPP

// --- Internal Includes ---
#include "AbsObserver.hpp"

// --- STD Includes ---
#include <memory>
#include <vector>


namespace cie {
namespace utils {


class AbsSubject
{
public:
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


}
}

#include "AbsSubject_impl.hpp"

#endif