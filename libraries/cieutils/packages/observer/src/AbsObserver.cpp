// --- Internal Ipmorts ---
#include "../inc/AbsObserver.hpp"
#include "../inc/AbsSubject.hpp"

namespace cie {
namespace utils {


AbsObserver::AbsObserver() :
    _subject( )
{
}


AbsObserver::AbsObserver( AbsSubjectPtr subject ) :
    _subject( subject )
{
}


AbsObserver::~AbsObserver()
{
    _subject.reset();
}


void AbsObserver::detach()
{
    if (_subject.lock() != nullptr && !_subject.expired())
    {
        _subject.lock()->detachObserver( shared_from_this() );
        _subject.lock().reset();
    }
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


}
}