// --- Internal Includes ---
#include "cieutils/packages/testing/inc/ConstructorTracker.hpp"


namespace cie::utils {


Size ConstructorTracker::_numberOfActiveInstances           = 0;
Size ConstructorTracker::_numberOfDefaultConstructorCalls   = 0;
Size ConstructorTracker::_numberOfCopyConstructorCalls      = 0;
Size ConstructorTracker::_numberOfCopyAssignmentCalls       = 0;
Size ConstructorTracker::_numberOfDestructorCalls           = 0;


ConstructorTracker::ConstructorTracker() :
    _localNumberOfConstructions( 1 )
{
    ConstructorTracker::_numberOfActiveInstances++;
    ConstructorTracker::_numberOfDefaultConstructorCalls++;
}


ConstructorTracker::ConstructorTracker( const ConstructorTracker& r_rhs ) :
    _localNumberOfConstructions( r_rhs._localNumberOfConstructions+1 )
{
    ConstructorTracker::_numberOfActiveInstances++;
    ConstructorTracker::_numberOfCopyConstructorCalls++;
}


ConstructorTracker& ConstructorTracker::operator=( const ConstructorTracker& r_rhs )
{
    this->_localNumberOfConstructions = r_rhs._localNumberOfConstructions+1;
    ConstructorTracker::_numberOfCopyAssignmentCalls++;
    return *this;
}


ConstructorTracker::~ConstructorTracker()
{
    ConstructorTracker::_numberOfActiveInstances--;
    ConstructorTracker::_numberOfDestructorCalls++;
}


const Size ConstructorTracker::numberOfActiveInstances()
{
    return ConstructorTracker::_numberOfActiveInstances;
}


const Size ConstructorTracker::numberOfDefaultConstructorCalls()
{
    return ConstructorTracker::_numberOfDefaultConstructorCalls;
}


const Size ConstructorTracker::numberOfCopyConstructorCalls()
{
    return ConstructorTracker::_numberOfCopyConstructorCalls;
}


const Size ConstructorTracker::numberOfCopyAssignmentCalls()
{
    return ConstructorTracker::_numberOfCopyAssignmentCalls;
}


const Size ConstructorTracker::numberOfDestructorCalls()
{
    return ConstructorTracker::_numberOfDestructorCalls;
}


} // namespace cie::utils