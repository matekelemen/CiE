// --- Internal Ipmorts ---
#include "cieutils/packages/observer/inc/AbsObserver.hpp"
#include "cieutils/packages/observer/inc/AbsSubject.hpp"
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/macros/inc/checks.hpp"

namespace cie::utils {


AbsObserver::AbsObserver() :
    _subject( )
{
}


AbsObserver::AbsObserver( AbsSubjectPtr subject ) :
    _subject( subject )
{
}


AbsObserver::AbsObserver( const AbsObserverPtr& copy ) :
    _subject( copy->_subject )
{
}


AbsObserverPtr AbsObserver::operator=( const AbsObserverPtr& copy )
{
    CIE_BEGIN_EXCEPTION_TRACING

    CIE_CHECK_POINTER( copy )
    _subject = copy->_subject;
    return shared_from_this();

    CIE_END_EXCEPTION_TRACING
}


AbsObserver::~AbsObserver()
{
    _subject.reset();
}


void AbsObserver::detach()
{
    CIE_BEGIN_EXCEPTION_TRACING

    if (_subject.lock() != nullptr && !_subject.expired())
    {
        _subject.lock()->detachObserver( shared_from_this() );
        _subject.lock().reset();
    }

    CIE_END_EXCEPTION_TRACING
}


void AbsObserver::setSubject( AbsSubjectPtr subject )
{
    _subject = subject;
}


AbsSubjectPtr AbsObserver::subject()
{
    return _subject.lock();
}


const AbsSubjectPtr AbsObserver::subject() const
{
    return _subject.lock();
}


} // namespace cie::utils