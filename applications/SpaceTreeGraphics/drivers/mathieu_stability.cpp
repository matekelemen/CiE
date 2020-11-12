
// --- GL Includes ---
#include <ciegl/ciegl.hpp>

// --- Internal Includes ---
#include "hilbert_determinant.hpp"
#include "complex.hpp"
#include "typedefs.hpp"
#include "PointScene.hpp"


// Define target function
const size_t dimension      = 3;
const size_t subdivision    = 5;
const size_t depth          = 7;

const double GAMMA          = 0.2;
const double OMEGA          = 1.0;
//const double OMEGACRITICAL  = 0.0;
const size_t N              = 5;
const size_t M              = 5;

auto targetFunction = [](const typename cie::csg::NodeType::point_type& parameters)
{   
    cie::mathieu::Complex determinant = cie::mathieu::hilbertDeterminant( 
                                            parameters[0],
                                            parameters[1],
                                            GAMMA,
                                            OMEGA,
                                            parameters[2]/10.0,
                                            N);
    double result = abs(determinant[0]) < abs(determinant[1]) ? determinant[0] : determinant[1];
    return result;
};


namespace cie::csg {

template <cie::detail::CubeType Node>
std::shared_ptr<Node> makeRoot()
{
    return std::make_shared<Node>(
        typename Node::sampler_ptr( new SamplerType(numberOfPointsPerDimension) ),
        typename Node::split_policy_ptr( new SplitterType ),
        0,
        PointType{0.0,0.0,0.0},
        10.0 );
}


template <cie::detail::BoxType Node>
std::shared_ptr<Node> makeRoot()
{
    return std::make_shared<Node>(
        typename Node::sampler_ptr( new SamplerType(numberOfPointsPerDimension) ),
        typename Node::split_policy_ptr( new SplitterType ),
        0,
        PointType{0.0,0.0,0.0},
        PointType{ 10.0, 10.0, 10.0 }
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
    auto p_context = gl::ContextPtr( new gl::GLFWContext(
        4,                                          // <-- OpenGL version major
        5,                                          // <-- OpenGL version minor
        0,                                          // <-- Number of MSAA samples
        OUTPUT_PATH + "/mathieu_stability_log.txt", // <-- log file
        true                                        // <-- use console output
    ) );

    auto p_window = p_context->newWindow(
        800,
        600
    );

    auto p_scene = std::make_shared<PointScene>( *p_context, "PointScene" );
    p_scene->addRoot( p_root );

    p_window->addScene( p_scene );

    auto p_camera = p_scene->getCamera();

    p_camera->setFieldOfView( 180.0 * M_PI/180.0 );
    p_camera->setClippingPlanes( 0.1, 100.0 );
    p_camera->setPosition( {0.0, 0.0, 10.0} );

    p_camera->rotateYaw( -2.0 * M_PI / 4.0 );
    p_camera->rotatePitch( -M_PI / 4.0 );

    //p_camera->rotateRoll( -3.0 * M_PI / 4.0 );
    //p_camera->rotate( M_PI / 4.0,
    //                  { -1.0, 1.0, 0.0 } );
    //p_camera->rotateRoll( -M_PI / 4.0 );

    // Start event loop
    p_window->update();
    std::cin.get();

    return 0;
}


} // namespace cie


int main()
{
    return cie::main();
}