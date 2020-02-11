#include "catch.hpp"
#include "../inc/GLContext.hpp"
#include <iostream>

TEST_CASE( "GLContext" )
{

    auto loopFactory = [] (cie::gl::GLContext& context) -> cie::gl::EventLoopFunction
    {
        int* loopCounter = new int(0);

        auto function = [&context, loopCounter]() -> void
        {
            context.logger().report( std::to_string(*loopCounter) );
            if (++(*loopCounter) > 8 )
            {
                delete(loopCounter);
                glfwSetWindowShouldClose( context.window(), 1 );
            }
        };

        return function;
    };


    cie::gl::GLContext context(     4,5,4,
                                    loopFactory, 
                                    "GLContextTest_logger.txt" );
    REQUIRE_NOTHROW( context.openWindow() );
    REQUIRE_NOTHROW( context.makeContextCurrent() );
    REQUIRE_NOTHROW( context.startEventLoop() );
}