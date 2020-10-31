
// --- CiE Includes ---
#include <ciegl/csgl.hpp>

// --- Internal Includes ---
#include "mergefunction.hpp"
#include "typedefs.hpp"

// --- STL Includes ---
#include <stdlib.h>


namespace cie::csg {



template <concepts::Cube Node>
Node makeRoot()
{
    return Node(    typename Node::sampler_ptr( new SamplerType(numberOfPointsPerDimension) ),
                    typename Node::split_policy_ptr( new SplitterType ),
                    0,
                    PointType{-2.0,-2.0,-2.0},
                    4.0 );
}


template <concepts::Box Node>
Node makeRoot()
{
    return Node(    typename Node::sampler_ptr( new SamplerType(numberOfPointsPerDimension) ),
                    typename Node::split_policy_ptr( new SplitterType ),
                    0,
                    PointType{-2.0,-2.0,-2.0},
                    PointType{ 4.0, 4.0, 4.0 } );
}



int main(std::function<ValueType(const PointType&,Double)> targetFunction, Double speed = 1.0, Size depth = 6)
{
    // Initialize target function
    mergeCounter    = 0;
    auto target     = [&](const PointType& r_point) -> double { return targetFunction(r_point, 0.0); };

    // Build tree
    NodeType root = makeRoot<NodeType>();
    root.divide(target,depth);

    // Context creation
    gl::GLFWContext context( 4,5,4,"dropMerge_log.txt" );
    context.useConsole(true);
    auto p_window = context.newWindow();
    context.focusWindow( p_window );

    // Draw manager setup
    gl::SpaceTreeDrawManager<NodeType> manager(root,context);
    double time = glfwGetTime();

    manager.setDrawFunction( [&manager, &root, &time, targetFunction, speed, depth]()
        { 
            if (glfwGetTime()-time > 1.0/144.0)
            {
                auto offset = 2.5 - speed/2.0*glfwGetTime();
                if (offset < 0.0)
                    return false;

                auto target = [&](const PointType& point) -> double 
                    { return targetFunction(point, offset); };

                auto timerID = manager.tic();
                root.clear();

                #pragma omp parallel
                {
                #pragma omp single
                {
                root.divide(target,depth);
                }
                }

                manager.toc( "Dividing took", timerID );
                manager.collectNodesToBuffer();
            }
            return true;
        } );
    manager.initialize();

    // Camera setup
    auto p_camera = gl::CameraPtr(
        new gl::ArcballCamera(context)
    );
    //manager.camera()    = camera;
    p_camera->setProperties(60.0, 0.1, 100.0);
    p_camera->setPose( glm::dvec3( 2.0, 2.0, 2.0 ), 
                       glm::dvec3( -1.0, -1.0, -1.0 ), 
                       glm::dvec3( 0.0, 0.0, 1.0 ) );
    //p_camera->setCenter( glm::dvec3( 0.0, 0.0, 0.0 ) );
    p_camera->updateTransformationMatrix();

    auto p_scene = p_window->makeScene();
    p_scene->addCamera( p_camera );

    // Bind callbacks
    //gl::KeyCallbackFunction keyCallback         = makeCallback( gl::ArcballCallbacks::keyCallback,
    //                                                            &manager );
    //gl::CursorCallbackFunction cursorCallback   = makeCallback( gl::ArcballCallbacks::cursorCallback,
    //                                                            &manager );
    //gl::MouseCallbackFunction mouseCallback     = makeCallback( gl::ArcballCallbacks::mouseCallback,
    //                                                            &manager );

    // Start event loop
    context.startEventLoop(
        std::bind(&gl::SpaceTreeDrawManager<NodeType>::makeDrawFunction, &manager, std::placeholders::_1)
    );
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
        {return cie::csg::exponentialMergeFunction<3>(r_point,offset);};

    cie::csg::main(targetFunction, speed, depth);
    std::cout << "\nTotal merge function calls: " << cie::csg::mergeCounter << "\n";

    return 0;
}