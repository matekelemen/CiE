// --- Internal Includes ---
#include "cieutils/packages/observer/inc/AbsSubject.hpp"
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- STL Includes ---
#include <memory>
#include <vector>


namespace cie::utils{


AbsSubject::AbsSubject( const AbsSubjectPtr& copy )
{
}


AbsSubjectPtr AbsSubject::operator=( const AbsSubjectPtr& copy )
{
    CIE_BEGIN_EXCEPTION_TRACING

    return shared_from_this();

    CIE_END_EXCEPTION_TRACING
}


AbsSubject::~AbsSubject()
{
    for (auto it=_observers.begin(); it!=_observers.end(); ++it)
        it->reset();
}


void AbsSubject::notifyObservers()
{
    CIE_BEGIN_EXCEPTION_TRACING

    for (auto it=_observers.begin(); it!=_observers.end(); ++it)
        (*it)->onSubjectChange();

    CIE_END_EXCEPTION_TRACING
}


int AbsSubject::attachObserver( AbsObserverPtr observer )
{
    CIE_BEGIN_EXCEPTION_TRACING

    int id = observerID(observer);

    if ( id == -1 )
    {
        _observers.push_back(observer);
        id = _observers.size()-1;
    }
    
    return id;

    CIE_END_EXCEPTION_TRACING
}


void AbsSubject::detachObserver( AbsObserverPtr observer )
{
    CIE_BEGIN_EXCEPTION_TRACING

    int id = observerID(observer);

    if ( observer->subject().get() == this )
        observer->subject().reset();

    if ( id != -1 )
        _observers.erase( _observers.begin()+id );

    CIE_END_EXCEPTION_TRACING
}


int AbsSubject::observerID( AbsObserverPtr observer )
{
    CIE_BEGIN_EXCEPTION_TRACING

    int id = -1;

    for ( size_t i=0; i<_observers.size(); ++i )
        if ( _observers[i].get() == observer.get() )
        {
            id = (int)i;
            break;
        }
    
    return id;

    CIE_END_EXCEPTION_TRACING
}


const std::vector<AbsObserverPtr>& AbsSubject::observers() const
{
    return _observers;
}


} // namespace cie::utils