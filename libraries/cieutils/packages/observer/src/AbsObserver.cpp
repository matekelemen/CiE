// --- Internal Ipmorts ---
#include "cieutils/packages/observer/inc/Observer.hpp"
#include "cieutils/packages/observer/inc/Subject.hpp"
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/macros/inc/checks.hpp"

namespace cie::utils::observer {


Observer::Observer() :
    _subject( )
{
}


Observer::Observer( SubjectPtr subject ) :
    _subject( subject )
{
}


Observer::Observer( const ObserverPtr& copy ) :
    _subject( copy->_subject )
{
}


ObserverPtr Observer::operator=( const ObserverPtr& copy )
{
    CIE_BEGIN_EXCEPTION_TRACING

    CIE_CHECK_POINTER( copy )
    _subject = copy->_subject;
    return shared_from_this();

    CIE_END_EXCEPTION_TRACING
}


Observer::~Observer()
{
    _subject.reset();
}


void Observer::detach()
{
    CIE_BEGIN_EXCEPTION_TRACING

    if (_subject.lock() != nullptr && !_subject.expired())
    {
        _subject.lock()->detachObserver( shared_from_this() );
        _subject.lock().reset();
    }

    CIE_END_EXCEPTION_TRACING
}


void Observer::setSubject( SubjectPtr subject )
{
    _subject = subject;
}


SubjectPtr Observer::subject()
{
    return _subject.lock();
}


const SubjectPtr Observer::subject() const
{
    return _subject.lock();
}


} // namespace cie::utils::observer