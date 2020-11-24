
// --- CiE Includes ---
#include <ciegl/ciegl.hpp>

// --- Internal Includes ---
#include "mergefunction.hpp"
#include "PointScene.hpp"
#include "typedefs.hpp"
#include "cmake_variables.hpp"

// --- STL Includes ---
#include <stdlib.h>
#include <chrono>
#include <thread>
#include <algorithm>


namespace cie::csg {



template <cie::detail::CubeType Node>
std::shared_ptr<Node> makeRoot()
{
    PointType base;
    std::fill( base.begin(),
               base.end(),
               -2.0 );

    return std::make_shared<Node>(
        typename Node::sampler_ptr( new SamplerType(numberOfPointsPerDimension) ),
        typename Node::split_policy_ptr( new SplitterType ),
        0,
        base,
        4.0 );
}


template <cie::detail::BoxType Node>
std::shared_ptr<Node> makeRoot()
{
    PointType base, end;
    std::fill( base.begin(),
               base.end(),
               -2.0 );
    std::fill( end.begin(),
               end.end(),
               4.0 );

    return std::make_shared<Node>(
        typename Node::sampler_ptr( new SamplerType(numberOfPointsPerDimension) ),
        typename Node::split_policy_ptr( new SplitterType ),
        0,
        base,
        end
    );
}



int main( std::function<ValueType(const PointType&,Double)> targetFunction,
          Double speed = 1.0,
          Size depth = 6 )
{
    // Initialize target function
    mergeCounter    = 0;
    auto target     = [&](const PointType& r_point) -> double { return targetFunction(r_point, 0.0); };

    // Build tree
    auto p_root = makeRoot<NodeType>();
    p_root->divide(target,depth);

    // Context creation
    auto p_context = gl::ContextPtr( new gl::GLFWContext(
        4,                                      // <-- OpenGL version major
        5,                                      // <-- OpenGL version minor
        0,                                      // <-- Number of MSAA samples
        OUTPUT_PATH / "drop_merge_log.txt",     // <-- log file
        true                                    // <-- use console output
    ));

    auto p_window = p_context->newWindow(
        //800,
        //600
    );

    // Scene for drawing space tree leaf node centers
    auto p_scene = std::make_shared<PointScene>( *p_context, "SpaceTreeScene" );
    p_window->addScene( p_scene );

    p_scene->addRoot( p_root );

    auto p_camera = p_scene->getCamera();

    p_camera->setPosition( {0.0, 0.0, 3.0} );

    if ( Dimension == 3 )
    {
        p_camera->rotateRoll( -3.0 * M_PI / 4.0 );
        p_camera->rotate( M_PI / 4.0,
                        { -1.0, 1.0, 0.0 } );
        p_camera->rotateRoll( -M_PI / 4.0 );
    }
    
    p_camera->setClippingPlanes( 1e-3, 1e2 );
    p_camera->setAspectRatio( p_window->getSize().first / double(p_window->getSize().second) );
    p_camera->setFieldOfView( 60.0 * M_PI / 180.0 );

    // Camera controls
    auto p_controls = gl::CameraControlsPtr(
        new gl::FlyCameraControls
    );

    p_controls->bind( p_window, p_camera );

    // Loop function setup
    auto t0   = std::chrono::high_resolution_clock::now();

    auto elapsed = []( decltype(t0)& r_time, bool update = false ) -> double
    {
        auto tmp = std::chrono::high_resolution_clock::now();
        double elapsedTime = std::chrono::duration_cast<std::chrono::duration<double>>(tmp-r_time).count();
        
        if ( update )
            r_time = tmp;

        return elapsedTime;
    };

    auto nextFrame = [&]() -> bool
    {
        // Drop center offset
        double offset = 2.5 - speed / 2.0 * elapsed(t0);
        
        if ( offset < 0.0 )
            return false;

        auto target = [&](const PointType& point) -> double 
        { return targetFunction(point, offset); };

        auto timerID = p_scene->tic();

        // Divide
        #pragma omp parallel
        {
        #pragma omp single
        {
        p_root->divide(target,depth);
        }
        }

        p_scene->toc( "Dividing",
                      timerID,
                      false );

        // Collect leaf centers
        p_scene->updatePoints();

        // Draw
        p_window->update();

        return true;
    };

    
    // Run loop
    while( nextFrame() ) {}


    return 0;
}

} // namespace cie::csg


int main(int argc, char *argv[])
{
    // Parse argument (animation speed)
    double speed    = 1.0;
    int depth       = 6;
    if (argc>1)
        speed = std::atof(argv[1]);
    if (argc>2)
        depth = std::atoi(argv[2]);

    auto targetFunction =  [](const cie::csg::PointType& r_point, double offset) 
        {return cie::csg::exponentialMergeFunction<cie::csg::Dimension>(r_point,offset);};

    cie::csg::main(targetFunction, speed, depth);
    std::cout << "\nTotal merge function calls: " << cie::csg::mergeCounter << "\n";

    return 0;
}