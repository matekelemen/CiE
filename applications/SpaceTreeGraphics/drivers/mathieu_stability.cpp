
// --- GL Includes ---
#include <ciegl/ciegl.hpp>

// --- Internal Includes ---
#include "hilbert_determinant.hpp"
#include "complex.hpp"
#include "typedefs.hpp"
#include "PointScene.hpp"

// --- STL Includes ---
#include <algorithm>


// Define target function
const size_t depth          = 6;

auto targetFunction = [](const typename cie::csg::NodeType::point_type& parameters)
{
    double gamma = 0.1;
    double omega = 1.0;
    // double omegaCritical = 0.0;

    cie::Complex determinant = cie::hilbertDeterminant( 
                                            parameters[0],
                                            parameters[1],
                                            gamma,
                                            omega,
                                            parameters[2]/10.0,
                                            cie::csg::Dimension);
    double result = std::abs(determinant.real()) < std::abs(determinant.imag()) ? determinant.real() : determinant.imag();
    return result;
};


namespace cie::csg {

template <cie::detail::CubeType Node>
std::shared_ptr<Node> makeRoot( Size numberOfPointsPerDimension = 5 )
{
    PointType base;
    std::fill( base.begin(),
               base.end(),
               0.1 );

    return std::make_shared<Node>(
        typename Node::sampler_ptr( new SamplerType(numberOfPointsPerDimension) ),
        typename Node::split_policy_ptr( new SplitterType ),
        0,
        base,
        8.0 );
}


template <cie::detail::BoxType Node>
std::shared_ptr<Node> makeRoot( Size numberOfPointsPerDimension = 5 )
{
    PointType base, end;
    std::fill( base.begin(),
               base.end(),
               0.1 );
    std::fill( end.begin(),
               end.end(),
               8.0 );

    return std::make_shared<Node>(
        typename Node::sampler_ptr( new SamplerType(numberOfPointsPerDimension) ),
        typename Node::split_policy_ptr( new SplitterType ),
        0,
        base,
        end
    );
}

} // namespace cie::csg



namespace cie {


int main()
{
    // Create root node and divide
    auto p_root = csg::makeRoot<csg::NodeType>();
    p_root->divide(targetFunction,depth);

    // Graphics setup
    auto p_context = gl::GLFWContextSingleton::get(
        4,                                          // <-- OpenGL version major
        5,                                          // <-- OpenGL version minor
        0,                                          // <-- Number of MSAA samples
        OUTPUT_PATH / "mathieu_stability_log.txt",  // <-- log file
        true                                        // <-- use console output
    );

    auto p_window = p_context->newWindow(
        800,
        600
    );

    auto p_scene = std::make_shared<PointScene>( *p_context, "PointScene" );
    p_scene->addRoot( p_root );

    p_window->addScene( p_scene );

    auto p_camera = p_scene->getCamera();
    
    auto p_cameraControls = std::make_shared<gl::FlyCameraControls>();
    p_cameraControls->bind( p_window, p_camera );
    p_cameraControls->setMovementScale( 1e-1 );

    p_camera->setFieldOfView( 90.0 * M_PI/180.0 );
    p_camera->setClippingPlanes( 0.1, 100.0 );
    p_camera->setPosition( {0.0, 0.0, 10.0} );

    p_camera->rotateYaw( -2.0 * M_PI / 4.0 );
    p_camera->rotatePitch( -M_PI / 4.0 );

    //p_camera->rotateRoll( -3.0 * M_PI / 4.0 );
    //p_camera->rotate( M_PI / 4.0,
    //                  { -1.0, 1.0, 0.0 } );
    //p_camera->rotateRoll( -M_PI / 4.0 );

    // Start event loop
    p_scene->updatePoints();
    p_window->beginLoop();

    return 0;
}


} // namespace cie


int main()
{
    return cie::main();
}