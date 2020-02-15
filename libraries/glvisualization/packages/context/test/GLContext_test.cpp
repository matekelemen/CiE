#include "catch.hpp"
#include "../inc/GLContext.hpp"
#include "../inc/DrawManager.hpp"
#include <iostream>


cie::gl::GLContext context_global( 4,5,4,"glvisualization_testrunner_log.txt" );


namespace cie {
namespace gl {

/*
TEST_CASE( "GLContext" )
{

    auto loopFactory = [] (cie::gl::GLContext& context) -> cie::gl::DrawFunction
    {
        int* loopCounter = new int(0);

        auto function = [&context, loopCounter]() -> void
        {
            context.log( std::to_string(*loopCounter) );
            if (++(*loopCounter) > 8 )
            {
                delete loopCounter;
                glfwSetWindowShouldClose( context.window(), 1 );
            }
        };

        return function;
    };

    REQUIRE_NOTHROW( context_global.openWindow() );
    REQUIRE_NOTHROW( context_global.makeContextCurrent() );
    REQUIRE_NOTHROW( context_global.startEventLoop( loopFactory ) );
}
*/




TEST_CASE( "DrawManager" )
{
    // Create context
    REQUIRE_NOTHROW( context_global.openWindow() );
    REQUIRE_NOTHROW( context_global.makeContextCurrent() );
    
    // Create draw manager
    DrawManager drawManager( context_global );
    REQUIRE_NOTHROW(drawManager.makeProgram());
    REQUIRE_NOTHROW(drawManager.initialize());

    // Start the event loop
    auto drawFunctionFactory = [&drawManager](GLContext& context_global)->DrawFunction 
    {
        auto function = [&drawManager] () ->void { drawManager.draw(); };
        return function;
    };
    REQUIRE_NOTHROW(context_global.startEventLoop( drawFunctionFactory ));
}



}
}