#ifndef CIE_UTILS_TESTING_CONSTRUCTOR_TRACKER_IMPL_HPP
#define CIE_UTILS_TESTING_CONSTRUCTOR_TRACKER_IMPL_HPP

// --- Internal Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"


namespace cie::utils {


template <class Tag>
Size ConstructorTracker<Tag>::_numberOfActiveInstances           = 0;

template <class Tag>
Size ConstructorTracker<Tag>::_numberOfDefaultConstructorCalls   = 0;

template <class Tag>
Size ConstructorTracker<Tag>::_numberOfCopyConstructorCalls      = 0;

template <class Tag>
Size ConstructorTracker<Tag>::_numberOfCopyAssignmentCalls       = 0;

template <class Tag>
Size ConstructorTracker<Tag>::_numberOfDestructorCalls           = 0;


template <class Tag>
ConstructorTracker<Tag>::ConstructorTracker() :
    _localNumberOfConstructions( 1 )
{
    ConstructorTracker<Tag>::_numberOfActiveInstances++;
    ConstructorTracker<Tag>::_numberOfDefaultConstructorCalls++;
}


template <class Tag>
ConstructorTracker<Tag>::ConstructorTracker( const ConstructorTracker& r_rhs ) :
    _localNumberOfConstructions( r_rhs._localNumberOfConstructions+1 )
{
    ConstructorTracker<Tag>::_numberOfActiveInstances++;
    ConstructorTracker<Tag>::_numberOfCopyConstructorCalls++;
}


template <class Tag>
ConstructorTracker<Tag>& ConstructorTracker<Tag>::operator=( const ConstructorTracker& r_rhs )
{
    this->_localNumberOfConstructions = r_rhs._localNumberOfConstructions+1;
    ConstructorTracker<Tag>::_numberOfCopyAssignmentCalls++;
    return *this;
}


template <class Tag>
ConstructorTracker<Tag>::~ConstructorTracker()
{
    ConstructorTracker<Tag>::_numberOfActiveInstances--;
    ConstructorTracker<Tag>::_numberOfDestructorCalls++;
}


template <class Tag>
const Size ConstructorTracker<Tag>::numberOfActiveInstances()
{
    return ConstructorTracker<Tag>::_numberOfActiveInstances;
}


template <class Tag>
const Size ConstructorTracker<Tag>::numberOfDefaultConstructorCalls()
{
    return ConstructorTracker<Tag>::_numberOfDefaultConstructorCalls;
}


template <class Tag>
const Size ConstructorTracker<Tag>::numberOfCopyConstructorCalls()
{
    return ConstructorTracker<Tag>::_numberOfCopyConstructorCalls;
}


template <class Tag>
const Size ConstructorTracker<Tag>::numberOfCopyAssignmentCalls()
{
    return ConstructorTracker<Tag>::_numberOfCopyAssignmentCalls;
}


template <class Tag>
const Size ConstructorTracker<Tag>::numberOfDestructorCalls()
{
    return ConstructorTracker<Tag>::_numberOfDestructorCalls;
}


template <class Tag>
void ConstructorTracker<Tag>::resetCounters()
{
    if ( ConstructorTracker<Tag>::_numberOfActiveInstances != 0 )
        CIE_THROW( Exception, "Cannot reset counters while instances exist" )

    ConstructorTracker<Tag>::_numberOfActiveInstances         = 0;
    ConstructorTracker<Tag>::_numberOfDefaultConstructorCalls = 0;
    ConstructorTracker<Tag>::_numberOfCopyConstructorCalls    = 0;
    ConstructorTracker<Tag>::_numberOfCopyAssignmentCalls     = 0;
    ConstructorTracker<Tag>::_numberOfDestructorCalls         = 0;
}


} // namespace cie::utils

#endif