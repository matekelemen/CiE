#include "catch.hpp"
#include "../inc/GLContext.hpp"
#include "../../drawing/inc/Camera.hpp"
#include "../inc/DrawManager.hpp"
#include <iostream>


cie::gl::GLContext context_global( 4,5,4,"glvisualization_testrunner_log.txt" );


namespace cie {
namespace gl {

TEST_CASE( "GLContext" )
{

    auto loopFactory = [] (cie::gl::GLContext& context) -> cie::gl::DrawFunction
    {
        int* loopCounter = new int(0);

        auto function = [&context, loopCounter]() -> void
        {
            context.log( std::to_string(*loopCounter) );
            if (++(*loopCounter) >= 9 )
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


/*
TEST_CASE( "Scene" )
{
    Scene scene( context_global );
    glm::vec3 test = { 5.0f, 4.0f, 3.0f };
    scene.setCameraPose( test, test );
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
    REQUIRE_NOTHROW(context_global.startEventLoop( std::bind(&DrawManager::makeDrawFunction, &drawManager, std::placeholders::_1) ));
}



}
}