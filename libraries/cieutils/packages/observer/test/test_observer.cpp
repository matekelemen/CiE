// --- External Imports ---
#include "catch.hpp"

// --- Internal Imports ---
#include "../inc/AbsObserver.hpp"
#include "../inc/AbsSubject.hpp"

// --- STD Imports ---
#include <iostream>

namespace cie {
namespace utils {


struct Observer : public AbsObserver
{
    void onSubjectChange( ) override {}
};
using ObserverPtr = std::shared_ptr<Observer>;


TEST_CASE( "AbsObserver" )
{
    // Create subject
    AbsSubjectPtr subject   = std::make_shared<AbsSubject>();

    // Create Observers
    ObserverPtr observer1   = std::make_shared<Observer>();
    ObserverPtr observer2   = std::make_shared<Observer>();

    REQUIRE( observer1->subject().get() == nullptr );
    REQUIRE_NOTHROW( observer1->onSubjectChange( ) );
    
    REQUIRE_NOTHROW( observer1->setSubject(subject) );
    REQUIRE( observer1->subject().get() == subject.get() );
    REQUIRE_NOTHROW( observer1->onSubjectChange( ) );

    // Check attaching a new observer
    int observerID = -1;
    REQUIRE_NOTHROW( observerID = subject->attachObserver( observer1 ) );
    REQUIRE( observerID == 0 );
    REQUIRE_NOTHROW( observerID = subject->attachObserver( observer2 ) );
    REQUIRE( observerID == 1 );

    // Check attaching an existing observer
    REQUIRE_NOTHROW( observerID = subject->attachObserver( observer1 ) );
    REQUIRE_NOTHROW( observerID = subject->observerID( observer1 ) );
    REQUIRE( observerID == 0 );

    REQUIRE_NOTHROW( observerID = subject->attachObserver( observer2 ) );
    REQUIRE_NOTHROW( observerID = subject->observerID( observer2 ) );
    REQUIRE( observerID == 1 );
}


}
}