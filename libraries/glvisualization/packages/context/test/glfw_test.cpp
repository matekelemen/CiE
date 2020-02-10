#include "catch.hpp"
#include "../inc/GLContext.hpp"

TEST_CASE( "Context" )
{
    cie::gl::GLContext context;
    REQUIRE_NOTHROW( context.openWindow() );
    REQUIRE_NOTHROW( context.makeContextCurrent() );
    REQUIRE_NOTHROW( context.startEventLoop() );
}