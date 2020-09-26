// --- External Imports ---
#include "catch.hpp"

// --- Internal Imports ---
#include "cieutils/packages/observer/inc/AbsObserver.hpp"
#include "cieutils/packages/observer/inc/AbsSubject.hpp"

// --- STD Imports ---
#include <iostream>
#include <typeinfo>

namespace cie::utils {


// Observer wrapper
struct Observer : public AbsObserver
{
    Observer(int* counter) : AbsObserver(), _counter(counter) {++(*_counter);}
    ~Observer() {--(*_counter);}
    void onSubjectChange( ) override {}

    int* _counter;
};
using ObserverPtr = std::shared_ptr<Observer>;


// Subject wrapper
struct Subject : public AbsSubject
{
    Subject(int* counter) : AbsSubject(), _counter(counter) {++(*_counter);}
    ~Subject() {--(*_counter);}

    int* _counter;
};
using SubjectPtr = std::shared_ptr<Subject>;



TEST_CASE( "AbsObserver" )
{
    // Create counters
    int numberOfSubjects    = 0;
    int numberOfObservers   = 0;

    {
        // Create subject
        SubjectPtr subject      = std::make_shared<Subject>(&numberOfSubjects);

        // Create Observers
        ObserverPtr observer1   = std::make_shared<Observer>(&numberOfObservers);
        ObserverPtr observer2   = std::make_shared<Observer>(&numberOfObservers);

        // Check initial state of wrappers
        REQUIRE( numberOfSubjects   == 1 );
        REQUIRE( numberOfObservers  == 2 );

        CHECK( observer1->subject().get() == nullptr );
        CHECK_NOTHROW( observer1->onSubjectChange( ) );
        
        CHECK_NOTHROW( observer1->setSubject(subject) );
        CHECK( observer1->subject().get() == subject.get() );
        CHECK_NOTHROW( observer1->onSubjectChange( ) );

        // Check attaching a new observer
        int observerID = -1;
        CHECK_NOTHROW( observerID = subject->attachObserver( observer1 ) );
        CHECK( observerID == 0 );
        CHECK_NOTHROW( observerID = subject->attachObserver( observer2 ) );
        CHECK( observerID == 1 );

        // Check attaching an existing observer
        CHECK_NOTHROW( observerID = subject->attachObserver( observer1 ) );
        CHECK_NOTHROW( observerID = subject->observerID( observer1 ) );
        CHECK( observerID == 0 );

        CHECK_NOTHROW( observerID = subject->attachObserver( observer2 ) );
        CHECK_NOTHROW( observerID = subject->observerID( observer2 ) );
        CHECK( observerID == 1 );

        // Check detaching existing observer
        CHECK_NOTHROW( observer1->detach() );
        CHECK( subject->observerID( observer1 ) == -1 );
        CHECK( subject->observerID( observer2 ) == 0 );

        // Check detaching non-existent observer
        CHECK_NOTHROW( observer1->detach() );
        CHECK( subject->observerID( observer1 ) == -1 );
        CHECK( subject->observerID( observer2 ) == 0 );

        // Check detaching observer from subject
        CHECK_NOTHROW( subject->detachObserver( observer2 ) );
        CHECK( subject->observerID( observer2 ) == -1 );
        CHECK( subject->observerID( observer1 ) == -1 );

        // Check casting
        auto absSubject = subject->as<AbsSubject>();
        CHECK( typeid(*absSubject) == typeid(*std::make_shared<AbsSubject>()) );
    }

    // Check final state of wrappers
    REQUIRE( numberOfSubjects   == 0 );
    REQUIRE( numberOfObservers  == 0 );
}


} // namespace cie::utils