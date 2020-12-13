#ifndef CIE_UTILS_TESTING_CONSTRUCTOR_TRACKER_HPP
#define CIE_UTILS_TESTING_CONSTRUCTOR_TRACKER_HPP

// --- Internal Includes ---
#include "cieutils/packages/types/inc/types.hpp"


namespace cie::utils {


class ConstructorTracker
{
public:
    ConstructorTracker();
    ConstructorTracker( const ConstructorTracker& r_rhs );
    ConstructorTracker& operator=( const ConstructorTracker& r_rhs );

    ~ConstructorTracker();

    const Size numberOfActiveInstances() const;

    static const Size numberOfDefaultConstructorCalls();
    static const Size numberOfCopyConstructorCalls();
    static const Size numberOfCopyAssignmentCalls();
    static const Size numberOfDestructorCalls();

private:
    Size _localNumberOfConstructions;

    static Size _numberOfActiveInstances;
    static Size _numberOfDefaultConstructorCalls;
    static Size _numberOfCopyConstructorCalls;
    static Size _numberOfCopyAssignmentCalls;
    static Size _numberOfDestructorCalls;
};


} // namespace cie::utils

#endif