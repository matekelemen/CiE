/*
// --- CSG Includes ---
#include <csg/trees.hpp>

// --- GL Includes ---
#include <glvisualization/csgvisualization.hpp>

// --- Internal Includes ---
#include "../inc/hilbert_determinant.hpp"
#include "../inc/complex.hpp"


// Define target function
const size_t dimension      = 3;
const size_t subdivision    = 5;
const size_t depth          = 7;

const double GAMMA          = 0.2;
const double OMEGA          = 1.0;
//const double OMEGACRITICAL  = 0.0;
const size_t N              = 5;
const size_t M              = 5;

auto targetFunction = [](const cie::DoubleArray<dimension>& parameters)
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


namespace cie {
namespace mathieu {


int main()
{
    // Define domain
    DoubleArray<dimension> center   = { 5.0, 5.0, 5.0 };
    double edgeLength               = 10.0;

    // Create root node and divide
    csg::SpaceTreeNode<dimension,subdivision> root( center, edgeLength );
    root.evaluate(targetFunction);
    root.divide(targetFunction,depth);

    // Graphics setup
    gl::GLContext context( 4,5,4,"mathieu_log.txt" );
    context.useConsole(true);
    context.openWindow();
    context.makeContextCurrent();

    gl::SpaceTreeDrawManager<5> manager(root,context);
    manager.setDrawFunction( [](){ return true; } );
    manager.initialize();

    auto camera         = std::make_shared<gl::ArcballCamera>(context);
    manager.camera()    = camera;
    camera->setProperties(60.0, 0.1, 100.0);
    camera->setPose(    glm::dvec3( center[0] + 2.0, center[1] + 2.0, center[2] + 2.0 ), 
                        glm::dvec3( -1.0, -1.0, -1.0 ), 
                        glm::dvec3( 0.0, 0.0, 1.0 ) );
    camera->setCenter( glm::dvec3( center[0], center[1], center[2] ) );
    camera->updateTransformationMatrix();

    gl::KeyCallbackFunction keyCallback         = makeCallback( gl::ArcballCallbacks::keyCallback,
                                                                &manager );
    gl::CursorCallbackFunction cursorCallback   = makeCallback( gl::ArcballCallbacks::cursorCallback,
                                                                &manager );
    gl::MouseCallbackFunction mouseCallback     = makeCallback( gl::ArcballCallbacks::mouseCallback,
                                                                &manager );

    // Start event loop
    context.startEventLoop( std::bind(&gl::SpaceTreeDrawManager<5>::makeDrawFunction, &manager, std::placeholders::_1),
                            keyCallback,
                            cursorCallback,
                            mouseCallback     );

    // Write data
    csg::writeSpaceTree<dimension,subdivision>(root, "mathieu_stability.csv");

    return 0;
}


}
}

*/
int main()
{
    //return cie::mathieu::main();
    return 0;
}