// --- CiE Includes ---
#include "glvisualization.hpp"

// --- Internal Includes ---
#include "mergefunction.hpp"
#include "SpaceTreeDrawManager.hpp"

// --- STD Includes ---
#include <stdlib.h>

namespace cie {
namespace csg {

// Problem settings
//const size_t dimension     = 3;
//const size_t subdivision   = 5;
const size_t depth         = 7;

//int main(std::function<double(const DoubleArray<dimension>&)> target, double offset = 0.0)
//{
//    SpaceTreeNode<dimension,subdivision> root( {0.0,0.0}, 4.0 );
//    root.evaluate(target);
//    root.divide(target,depth);
//
//    writeSpaceTree<dimension,subdivision>(root, "dropmerge.csv");
//
//    std::string filename    = "install/applications/dropmerge/dropmerge_csv.py";
//    std::string command     = "python3 ";
//    command                 += filename;
//    system(command.c_str());
//
//    return 0;
//}

int main(std::function<double(const DoubleArray<3>&)> target, double offset = 0.0)
{
    // Build tree
    SpaceTreeNode<3,M> root( {0.0,0.0,0.0}, 4.0 );
    root.evaluate(target);
    root.divide(target,depth);


    // Context creation
    gl::GLContext context( 4,5,4,"dropMerge_log.txt" );
    context.useConsole(true);
    context.openWindow();
    context.makeContextCurrent();

    // Draw manager setup
    SpaceTreeDrawManager manager(root,context);
    manager.setDrawFunction( [&manager]()
        { 
            if (glfwGetTime() > 1.0f/144.0)
            {
                //manager.camera()->rotate(0.01f,glm::vec3(1.0f,0.0f,0.0f),glm::vec3(0.0f,0.0f,0.0f)); 
                glfwSetTime( 0.0 );
            }
        } );
    manager.initialize();

    // Camera setup
    auto camera         = std::make_shared<gl::ArcballCamera>(context);
    manager.camera()    = camera;
    camera->setProperties(0.0f, 0.1f, 100.0f);
    camera->setPose( glm::vec3( 0.0f, 0.0f, 1.0f), glm::vec3( 0.0f, 0.0f, -1.0f ) );
    camera->setCenter( glm::vec3( 0.0f, 0.0f, 0.0f ) );
    camera->updateTransformationMatrix();

    // Bind callbacks
    gl::KeyCallbackFunction keyCallback         = makeCallback( gl::ArcballCallbacks::keyCallback,
                                                                &manager );
    gl::CursorCallbackFunction cursorCallback   = makeCallback( gl::ArcballCallbacks::cursorCallback,
                                                                &manager );
    gl::MouseCallbackFunction mouseCallback     = makeCallback( gl::ArcballCallbacks::mouseCallback,
                                                                &manager );

    // Start event loop
    context.startEventLoop( std::bind(&SpaceTreeDrawManager::makeDrawFunction, &manager, std::placeholders::_1),
                            keyCallback,
                            cursorCallback,
                            mouseCallback     );


    return 0;
}

}
}


int main(int argc, char *argv[])
{
    // Parse argument
    double offset = 0.0;
    if (argc>1)
        offset = std::atof(argv[1]);

    auto target =  [=](const cie::DoubleArray<3>& point) 
        {return cie::csg::exponentialMergeFunction<3>(point,offset);};


    return cie::csg::main(target, offset);
}