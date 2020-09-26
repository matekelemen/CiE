// --- Internal Includes ---
#include "cieutils/packages/observer/inc/AbsSubject.hpp"

// --- STL Includes ---
#include <memory>
#include <vector>


namespace cie::utils{


AbsSubject::AbsSubject( const AbsSubjectPtr& copy )
{
}


AbsSubjectPtr AbsSubject::operator=( const AbsSubjectPtr& copy )
{
    return shared_from_this();
}


AbsSubject::~AbsSubject()
{
    for (auto it=_observers.begin(); it!=_observers.end(); ++it)
        it->reset();
}


void AbsSubject::notifyObservers()
{
    for (auto it=_observers.begin(); it!=_observers.end(); ++it)
        (*it)->onSubjectChange();
}


int AbsSubject::attachObserver( AbsObserverPtr observer )
{
    int id = observerID(observer);

    if ( id == -1 )
    {
        _observers.push_back(observer);
        id = _observers.size()-1;
    }
    
    return id;
}


void AbsSubject::detachObserver( AbsObserverPtr observer )
{
    int id = observerID(observer);

    if ( observer->subject().get() == this )
        observer->subject().reset();

    if ( id != -1 )
        _observers.erase( _observers.begin()+id );
}


int AbsSubject::observerID( AbsObserverPtr observer )
{
    int id = -1;

    for ( size_t i=0; i<_observers.size(); ++i )
        if ( _observers[i].get() == observer.get() )
        {
            id = (int)i;
            break;
        }
    
    return id;
}


const std::vector<AbsObserverPtr>& AbsSubject::observers() const
{
    return _observers;
}


} // namespace cie::utils