// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "ciegl/packages/context/inc/GLFWWindow.hpp"
#include "ciegl/packages/context/inc/GLFWContext.hpp"
#include "ciegl/packages/shapes/inc/Arrow.hpp"
#include "ciegl/packages/buffer/inc/Vertex3.hpp"
#include "ciegl/packages/file/inc/GenericPart.hpp"
#include "ciegl/packages/scene/inc/GenericPartScene.hpp"
#include "ciegl/packages/camera/inc/Camera.hpp"
#include "ciegl/packages/camera/inc/OrthographicProjection.hpp"
#include "cmake_variables.hpp"

// --- STL Includes ---
#include <chrono>
#include <thread>


namespace cie::gl {


CIE_TEST_CASE( "Arrow", "[shapes]" )
{
    CIE_TEST_CASE_INIT( "Arrow" )

    auto p_context = GLFWContextSingleton::get();
    auto p_window  = p_context->newWindow();

    auto p_scene   = p_window->makeScene<GenericPartScene>(
        "ArrowScene",
        GL_TRIANGLES,
        SOURCE_PATH / "libraries/ciegl/data/shaders/Triangulated3DPartScene"
    );

    auto p_camera = p_scene->makeCamera<Camera<OrthographicProjection>>();

    p_camera->setPose( {1.2, 0.0, 0.8},
                       {0.0, 0.0, -1.0},
                       {0.0, 1.0, 0.0} );
    p_camera->rotateYaw( 30.0 * 3.14159265 / 180.0 );
    p_camera->setFieldOfView( 90.0 * 3.14159265 / 180.0 );
    p_camera->setClippingPlanes( 1e-3, 1e2 );

    p_scene->bindUniform( "transformation", p_camera->transformationMatrix() );
    p_scene->bindUniform( "cameraPosition", p_camera->position() );

    using VertexType = Vertex3;
    using ArrowType  = Arrow<VertexType>;

    auto p_part = std::make_shared<GenericPart>( 3, 3, 3 );

    CIE_TEST_CHECK_NOTHROW( ArrowType( p_part->attributes(),
                                       {0.0,0.0,0.0},
                                       {1.0,0.0,0.0} ) );
    ArrowType arrow( p_part->attributes(),
                     {0.0, 0.0, 0.0},
                     {1.0, 1.0, 1.0} );
    p_part->indices() = arrow.indices();

    p_scene->addPart( p_part );
    p_window->update();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
}


} // namespace cie::gl