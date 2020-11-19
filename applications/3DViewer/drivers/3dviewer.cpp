// --- Graphics Includes ---
#include <ciegl/ciegl.hpp>

// --- Utility Includes ---
#include <cieutils/commandline.hpp>

// --- Internal Includes ---
#include "ViewerScene.hpp"

// --- STL Includes ---
#include <deque>
#include <iostream>
#include <filesystem>


namespace cie {



void configureGLFWWindow( gl::WindowPtr p_window )
{
    GLFWwindow* p_GLFWWindow = std::dynamic_pointer_cast<gl::GLFWWindow>(p_window)->get();

    glfwSetInputMode(p_GLFWWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(p_GLFWWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}




int main( int argc, char const* argv[] )
{
    // Parse command line arguments
    utils::CommandLineArguments args( argc, argv );
    auto filePath = args.get<std::string>(0);

    // Graphics setup
    auto p_context = gl::ContextPtr( new gl::GLFWContext(
        4,                                      // <-- OpenGL version major
        5,                                      // <-- OpenGL version minor
        4,                                      // <-- Number of MSAA samples
        OUTPUT_PATH + "/3dviewer_log.txt",      // <-- log file
        true                                    // <-- use console output
    ) );

    auto p_window = p_context->newWindow(
        1024,
        768
    );
    configureGLFWWindow( p_window );

    auto p_scene = std::make_shared<ViewerScene>( *p_context );
    p_window->addScene( p_scene );

    // Load geometry
    // TODO: add support for other file types too
    for ( const auto& r_filePath : args.arguments() )
    {
        auto timerID = p_scene->tic();
        p_scene->log( "Loading " + r_filePath );

        gl::PartPtr p_model;

        std::filesystem::path filePath( r_filePath );
        auto extension = filePath.extension();

        if ( extension == ".stl" )
            p_model.reset( 
                new gl::STLPart( r_filePath )
                );
        else if ( extension == ".obj" )
            p_model.reset(
                new gl::ObjPart( r_filePath )
                );
        else
            CIE_THROW( Exception, "Unsupported file format: " + std::string(extension) )

        p_scene->toc( "Finished loading " + r_filePath, timerID );

        p_scene->addModel( p_model );
    }

    // Set initial camera pose
    auto p_camera = p_scene->getCamera();

    p_camera->setPose( {100.0, 100.0, 50.0},
                       {-1.0, -1.0, 0.0},
                       {0.0, 0.0, 1.0} );
    p_camera->setFieldOfView( 90 * M_PI / 180.0 );
    p_camera->setClippingPlanes( 1.0,
                                 300.0 );

    // Set controls
    auto p_controls = gl::CameraControlsPtr(
        new gl::FlyCameraControls
    );
    p_controls->bind( p_window, p_camera );

    p_window->beginLoop();

    return 0;
}


} // namespace cie







int main( int argc, char const* argv[] )
{
    return cie::main( argc, argv );
}