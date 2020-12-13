// --- Internal Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"
#include "cieutils/packages/testing/inc/ConstructorTracker.hpp"

// --- STL Includes ---
#include <vector>


namespace cie::utils {


CIE_TEST_CASE( "ConstructorTracker", "[testing]" )
{
    CIE_TEST_CASE_INIT( "ConstructorTracker" )

    CIE_TEST_REQUIRE_NOTHROW( ConstructorTracker() );
    ConstructorTracker tracker0;

    CIE_TEST_CHECK( ConstructorTracker::numberOfActiveInstances() == 1 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfDefaultConstructorCalls() == 2 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfCopyConstructorCalls() == 0 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfCopyAssignmentCalls() == 0 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfDestructorCalls() == 1 );

    {
        ConstructorTracker tmpTracker;
        CIE_TEST_CHECK( ConstructorTracker::numberOfActiveInstances() == 2 );
        CIE_TEST_CHECK( ConstructorTracker::numberOfDefaultConstructorCalls() == 3 );
        CIE_TEST_CHECK( ConstructorTracker::numberOfCopyConstructorCalls() == 0 );
        CIE_TEST_CHECK( ConstructorTracker::numberOfCopyAssignmentCalls() == 0 );
        CIE_TEST_CHECK( ConstructorTracker::numberOfDestructorCalls() == 1 );
    }

    CIE_TEST_CHECK( ConstructorTracker::numberOfActiveInstances() == 1 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfDefaultConstructorCalls() == 3 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfCopyConstructorCalls() == 0 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfCopyAssignmentCalls() == 0 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfDestructorCalls() == 2 );

    auto tracker1 = tracker0;
    CIE_TEST_CHECK( ConstructorTracker::numberOfActiveInstances() == 2 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfDefaultConstructorCalls() == 3 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfCopyConstructorCalls() == 1 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfCopyAssignmentCalls() == 0 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfDestructorCalls() == 2 );

    ConstructorTracker tracker2( tracker1 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfActiveInstances() == 3 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfDefaultConstructorCalls() == 3 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfCopyConstructorCalls() == 2 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfCopyAssignmentCalls() == 0 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfDestructorCalls() == 2 );

    tracker2 = tracker0;
    CIE_TEST_CHECK( ConstructorTracker::numberOfActiveInstances() == 3 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfDefaultConstructorCalls() == 3 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfCopyConstructorCalls() == 2 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfCopyAssignmentCalls() == 1 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfDestructorCalls() == 2 );

    std::vector<ConstructorTracker> trackers;
    trackers.reserve(100);
    CIE_TEST_CHECK( ConstructorTracker::numberOfActiveInstances() == 3 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfDefaultConstructorCalls() == 3 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfCopyConstructorCalls() == 2 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfCopyAssignmentCalls() == 1 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfDestructorCalls() == 2 );

    trackers.resize(1);
    CIE_TEST_CHECK( ConstructorTracker::numberOfActiveInstances() == 4 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfDefaultConstructorCalls() == 4 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfCopyConstructorCalls() == 2 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfCopyAssignmentCalls() == 1 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfDestructorCalls() == 2 );

    trackers.emplace_back();
    CIE_TEST_CHECK( ConstructorTracker::numberOfActiveInstances() == 5 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfDefaultConstructorCalls() == 5 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfCopyConstructorCalls() == 2 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfCopyAssignmentCalls() == 1 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfDestructorCalls() == 2 );

    trackers.push_back( tracker0 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfActiveInstances() == 6 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfDefaultConstructorCalls() == 5 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfCopyConstructorCalls() == 3 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfCopyAssignmentCalls() == 1 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfDestructorCalls() == 2 );

    trackers.emplace_back( tracker0 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfActiveInstances() == 7 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfDefaultConstructorCalls() == 5 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfCopyConstructorCalls() == 4 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfCopyAssignmentCalls() == 1 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfDestructorCalls() == 2 );

    trackers.back() = tracker0;
    CIE_TEST_CHECK( ConstructorTracker::numberOfActiveInstances() == 7 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfDefaultConstructorCalls() == 5 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfCopyConstructorCalls() == 4 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfCopyAssignmentCalls() == 2 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfDestructorCalls() == 2 );

    trackers.clear();
    CIE_TEST_CHECK( ConstructorTracker::numberOfActiveInstances() == 3 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfDefaultConstructorCalls() == 5 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfCopyConstructorCalls() == 4 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfCopyAssignmentCalls() == 2 );
    CIE_TEST_CHECK( ConstructorTracker::numberOfDestructorCalls() == 6 );
}


} // namespace cie::utils