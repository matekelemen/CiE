// --- Graphics Includes ---
#include <ciegl/ciegl.hpp>

// --- Utility Includes ---
#include <cieutils/commandline.hpp>

// --- Internal Includes ---
#include "ViewerScene.hpp"

// --- STL Includes ---
#include <deque>
#include <iostream>


namespace cie {


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

    auto p_scene = std::make_shared<ViewerScene>( *p_context );
    p_window->addScene( p_scene );

    // Load geometry
    // TODO: add support for other file types too
    for ( const auto& r_filePath : args.arguments() )
    {
        p_scene->log( "Loading " + r_filePath );

        auto p_model = gl::STLPartPtr( new gl::STLPart(
            r_filePath
        ) );
        //p_model->repairOrientation();

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
        new gl::FlyCameraControls( p_window, p_camera )
    );

    p_window->beginLoop();

    return 0;
}


} // namespace cie







int main( int argc, char const* argv[] )
{
    return cie::main( argc, argv );
}