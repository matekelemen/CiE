// --- Internal Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"
#include "cieutils/packages/testing/inc/ConstructorTracker.hpp"

// --- STL Includes ---
#include <vector>


namespace cie::utils {


struct ConstructorTrackerTestTag {};


CIE_TEST_CASE( "ConstructorTracker", "[testing]" )
{
    CIE_TEST_CASE_INIT( "ConstructorTracker" )

    using Tracker = ConstructorTracker<ConstructorTrackerTestTag>;

    CIE_TEST_CHECK_NOTHROW( Tracker::resetCounters() );
    CIE_TEST_CHECK( Tracker::numberOfActiveInstances() == 0 );
    CIE_TEST_CHECK( Tracker::numberOfDefaultConstructorCalls() == 0 );
    CIE_TEST_CHECK( Tracker::numberOfCopyConstructorCalls() == 0 );
    CIE_TEST_CHECK( Tracker::numberOfCopyAssignmentCalls() == 0 );
    CIE_TEST_CHECK( Tracker::numberOfDestructorCalls() == 0 );

    {
        CIE_TEST_REQUIRE_NOTHROW( Tracker() );
        Tracker tmpTracker;
        CIE_TEST_CHECK( Tracker::numberOfActiveInstances() == 1 );
        CIE_TEST_CHECK( Tracker::numberOfDefaultConstructorCalls() == 2 );
        CIE_TEST_CHECK( Tracker::numberOfCopyConstructorCalls() == 0 );
        CIE_TEST_CHECK( Tracker::numberOfCopyAssignmentCalls() == 0 );
        CIE_TEST_CHECK( Tracker::numberOfDestructorCalls() == 1 );
        CIE_TEST_CHECK_THROWS( Tracker::resetCounters() );
    }

    CIE_TEST_CHECK_NOTHROW( Tracker::resetCounters() );
    CIE_TEST_CHECK( Tracker::numberOfActiveInstances() == 0 );
    CIE_TEST_CHECK( Tracker::numberOfDefaultConstructorCalls() == 0 );
    CIE_TEST_CHECK( Tracker::numberOfCopyConstructorCalls() == 0 );
    CIE_TEST_CHECK( Tracker::numberOfCopyAssignmentCalls() == 0 );
    CIE_TEST_CHECK( Tracker::numberOfDestructorCalls() == 0 );


    CIE_TEST_REQUIRE_NOTHROW( Tracker() );
    Tracker tracker0;

    CIE_TEST_CHECK( Tracker::numberOfActiveInstances() == 1 );
    CIE_TEST_CHECK( Tracker::numberOfDefaultConstructorCalls() == 2 );
    CIE_TEST_CHECK( Tracker::numberOfCopyConstructorCalls() == 0 );
    CIE_TEST_CHECK( Tracker::numberOfCopyAssignmentCalls() == 0 );
    CIE_TEST_CHECK( Tracker::numberOfDestructorCalls() == 1 );

    {
        Tracker tmpTracker;
        CIE_TEST_CHECK( Tracker::numberOfActiveInstances() == 2 );
        CIE_TEST_CHECK( Tracker::numberOfDefaultConstructorCalls() == 3 );
        CIE_TEST_CHECK( Tracker::numberOfCopyConstructorCalls() == 0 );
        CIE_TEST_CHECK( Tracker::numberOfCopyAssignmentCalls() == 0 );
        CIE_TEST_CHECK( Tracker::numberOfDestructorCalls() == 1 );
    }

    CIE_TEST_CHECK( Tracker::numberOfActiveInstances() == 1 );
    CIE_TEST_CHECK( Tracker::numberOfDefaultConstructorCalls() == 3 );
    CIE_TEST_CHECK( Tracker::numberOfCopyConstructorCalls() == 0 );
    CIE_TEST_CHECK( Tracker::numberOfCopyAssignmentCalls() == 0 );
    CIE_TEST_CHECK( Tracker::numberOfDestructorCalls() == 2 );

    auto tracker1 = tracker0;
    CIE_TEST_CHECK( Tracker::numberOfActiveInstances() == 2 );
    CIE_TEST_CHECK( Tracker::numberOfDefaultConstructorCalls() == 3 );
    CIE_TEST_CHECK( Tracker::numberOfCopyConstructorCalls() == 1 );
    CIE_TEST_CHECK( Tracker::numberOfCopyAssignmentCalls() == 0 );
    CIE_TEST_CHECK( Tracker::numberOfDestructorCalls() == 2 );

    Tracker tracker2( tracker1 );
    CIE_TEST_CHECK( Tracker::numberOfActiveInstances() == 3 );
    CIE_TEST_CHECK( Tracker::numberOfDefaultConstructorCalls() == 3 );
    CIE_TEST_CHECK( Tracker::numberOfCopyConstructorCalls() == 2 );
    CIE_TEST_CHECK( Tracker::numberOfCopyAssignmentCalls() == 0 );
    CIE_TEST_CHECK( Tracker::numberOfDestructorCalls() == 2 );

    tracker2 = tracker0;
    CIE_TEST_CHECK( Tracker::numberOfActiveInstances() == 3 );
    CIE_TEST_CHECK( Tracker::numberOfDefaultConstructorCalls() == 3 );
    CIE_TEST_CHECK( Tracker::numberOfCopyConstructorCalls() == 2 );
    CIE_TEST_CHECK( Tracker::numberOfCopyAssignmentCalls() == 1 );
    CIE_TEST_CHECK( Tracker::numberOfDestructorCalls() == 2 );

    std::vector<Tracker> trackers;
    trackers.reserve(100);
    CIE_TEST_CHECK( Tracker::numberOfActiveInstances() == 3 );
    CIE_TEST_CHECK( Tracker::numberOfDefaultConstructorCalls() == 3 );
    CIE_TEST_CHECK( Tracker::numberOfCopyConstructorCalls() == 2 );
    CIE_TEST_CHECK( Tracker::numberOfCopyAssignmentCalls() == 1 );
    CIE_TEST_CHECK( Tracker::numberOfDestructorCalls() == 2 );

    trackers.resize(1);
    CIE_TEST_CHECK( Tracker::numberOfActiveInstances() == 4 );
    CIE_TEST_CHECK( Tracker::numberOfDefaultConstructorCalls() == 4 );
    CIE_TEST_CHECK( Tracker::numberOfCopyConstructorCalls() == 2 );
    CIE_TEST_CHECK( Tracker::numberOfCopyAssignmentCalls() == 1 );
    CIE_TEST_CHECK( Tracker::numberOfDestructorCalls() == 2 );

    trackers.emplace_back();
    CIE_TEST_CHECK( Tracker::numberOfActiveInstances() == 5 );
    CIE_TEST_CHECK( Tracker::numberOfDefaultConstructorCalls() == 5 );
    CIE_TEST_CHECK( Tracker::numberOfCopyConstructorCalls() == 2 );
    CIE_TEST_CHECK( Tracker::numberOfCopyAssignmentCalls() == 1 );
    CIE_TEST_CHECK( Tracker::numberOfDestructorCalls() == 2 );

    trackers.push_back( tracker0 );
    CIE_TEST_CHECK( Tracker::numberOfActiveInstances() == 6 );
    CIE_TEST_CHECK( Tracker::numberOfDefaultConstructorCalls() == 5 );
    CIE_TEST_CHECK( Tracker::numberOfCopyConstructorCalls() == 3 );
    CIE_TEST_CHECK( Tracker::numberOfCopyAssignmentCalls() == 1 );
    CIE_TEST_CHECK( Tracker::numberOfDestructorCalls() == 2 );

    trackers.emplace_back( tracker0 );
    CIE_TEST_CHECK( Tracker::numberOfActiveInstances() == 7 );
    CIE_TEST_CHECK( Tracker::numberOfDefaultConstructorCalls() == 5 );
    CIE_TEST_CHECK( Tracker::numberOfCopyConstructorCalls() == 4 );
    CIE_TEST_CHECK( Tracker::numberOfCopyAssignmentCalls() == 1 );
    CIE_TEST_CHECK( Tracker::numberOfDestructorCalls() == 2 );

    trackers.back() = tracker0;
    CIE_TEST_CHECK( Tracker::numberOfActiveInstances() == 7 );
    CIE_TEST_CHECK( Tracker::numberOfDefaultConstructorCalls() == 5 );
    CIE_TEST_CHECK( Tracker::numberOfCopyConstructorCalls() == 4 );
    CIE_TEST_CHECK( Tracker::numberOfCopyAssignmentCalls() == 2 );
    CIE_TEST_CHECK( Tracker::numberOfDestructorCalls() == 2 );

    trackers.clear();
    CIE_TEST_CHECK( Tracker::numberOfActiveInstances() == 3 );
    CIE_TEST_CHECK( Tracker::numberOfDefaultConstructorCalls() == 5 );
    CIE_TEST_CHECK( Tracker::numberOfCopyConstructorCalls() == 4 );
    CIE_TEST_CHECK( Tracker::numberOfCopyAssignmentCalls() == 2 );
    CIE_TEST_CHECK( Tracker::numberOfDestructorCalls() == 6 );


    CIE_TEST_CHECK_THROWS( Tracker::resetCounters() );
}


} // namespace cie::utils