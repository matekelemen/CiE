// --- Graphics Includes ---
#include <ciegl/ciegl.hpp>

// --- Mesh Includes ---
#include <meshkernel/marchingprimitives.hpp>

// --- CSG Includes ---
#include <csg/primitives.hpp>

// --- Utility Includes ---
#include <cieutils/macros.hpp>

// --- Internal Includes ---
#include "ViewerScene.hpp"
#include "cmake_variables.hpp"


namespace cie {

/* --- TypeDefs --- */

const Size Dimension  = 3;
using CoordinateType  = double;
using TargetValueType = Bool;

using CSGObject           = csg::CSGObject<Dimension,TargetValueType,CoordinateType>;
using CSGObjectPtr        = csg::CSGObjectPtr<Dimension,TargetValueType,CoordinateType>;

using Traits              = mesh::MeshTraits<Dimension,CoordinateType>;
using PointType           = Traits::point_type;
using DomainSpecifier     = Traits::domain_specifier;
using ResolutionSpecifier = Traits::resolution_specifier;
using MarchingCubes       = mesh::MarchingCubes<CSGObject>;



class MarchingPart : public gl::Triangulated3DPart
{
public:
    MarchingPart( CSGObjectPtr p_target,
                  const PointType& r_origin,
                  const ResolutionSpecifier& r_numberOfCubes,
                  CoordinateType edgeLength )
    {
        Size vertexCounter = 0;

        MarchingCubes::output_functor pushTriangle = [&vertexCounter,this]( const MarchingCubes::output_arguments& r_triangle ) -> void
        {
            for ( const auto& r_vertex : r_triangle )
            {
                for ( MarchingPart::data_type component : r_vertex )
                    this->_data.push_back( component );

                this->_indices.push_back( vertexCounter++ );
            }
        };

        MarchingCubes(
            p_target,
            r_origin,
            r_numberOfCubes,
            edgeLength,
            pushTriangle
        ).execute();
    }
};


int main( int argc, char const* argv[] )
{
    // Construct target
    auto p_target = CSGObjectPtr(
        new csg::boolean::Sphere<Dimension,CoordinateType>( {0.0,0.0}, 1.0 )
    );

    // Graphics setup
    auto p_context = gl::GLFWContextSingleton::get(
        OUTPUT_PATH / "csg2stl.log",                    // <-- log file
        true                                            // use console output
    );

    auto p_window = p_context->newWindow( 1024, 768 );
    auto p_scene  = std::make_shared<ViewerScene>( *p_context );
    p_window->addScene( p_scene);

    // March and add model
    auto timerID = p_scene->tic();

    auto p_model = gl::PartPtr( new MarchingPart(
        p_target,
        { 0.0, 0.0, 0.0 },
        { 1, 1, 1 },
        0.999
    ) );

    p_scene->toc( "Finished scanning", timerID );
    p_model->writeSTL( OUTPUT_PATH / "csg2stl.stl" );

    p_scene->addModel( p_model );

    // Set initial camera pose
    auto p_camera = p_scene->getCamera();
    p_camera->setAspectRatio( double(p_window->getSize().first) / p_window->getSize().second );

    p_camera->setPose( {2.0, 2.0, 0.0},
                       {-1.0, -1.0, 0.0},
                       {0.0, 0.0, 1.0} );
    p_camera->setFieldOfView( 90 * M_PI / 180.0 );
    p_camera->setClippingPlanes( 1e-3,
                                 50.0 );

    // Set controls
    auto p_controls = std::make_shared<gl::ArcBallCameraControls>(true);
    p_controls->bind( p_window, p_camera );

    p_controls->setZoomScale( 0.01 );
    p_controls->setRotationScale( 0.005 );

    // Event loop
    p_window->beginLoop();

    return 0;
}


} // namespace cie





int main( int argc, char const* argv[] )
{
    return cie::main( argc, argv );
}