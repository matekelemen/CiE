#define _USE_MATH_DEFINES

// --- Graphics Includes ---
#include <ciegl/ciegl.hpp>

// --- Mesh Includes ---
#include <meshkernel/marchingprimitives.hpp>
#include <meshkernel/structured.hpp>

// --- CSG Includes ---
#include <csg/primitives.hpp>
#include <csg/operators.hpp>

// --- LinAlg Includes ---
#include <linalg/overloads.hpp>

// --- Utility Includes ---
#include <cieutils/macros.hpp>

// --- Internal Includes ---
#include "ViewerScene.hpp"
#include "cmake_variables.hpp"

// --- STL Includes ---
#include <mutex>


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
using MarchingCubes       = mesh::StructuredMarchingCubes<CSGObject>;



class MarchingPart : public gl::Triangulated3DPart
{
public:
    MarchingPart( CSGObjectPtr p_target,
                  const PointType& r_origin,
                  const ResolutionSpecifier& r_numberOfCubes,
                  CoordinateType edgeLength )
    {
        Size vertexCounter = 0;

        // Pull in linalg operator overloads from the global namespace
        using ::operator+;
        using ::operator/;

        MarchingCubes::domain_specifier domain {{
            { r_origin[0], r_origin[0] + edgeLength * r_numberOfCubes[0] },
            { r_origin[1], r_origin[1] + edgeLength * r_numberOfCubes[1] },
            { r_origin[2], r_origin[2] + edgeLength * r_numberOfCubes[2] }
        }};

        MarchingCubes::resolution_specifier numberOfPoints {{
            r_numberOfCubes[0] + 1,
            r_numberOfCubes[1] + 1,
            r_numberOfCubes[2] + 1
        }};

        MarchingCubes marchingCubes(
            p_target,
            domain,
            numberOfPoints,
            nullptr
        );

        std::mutex mutex;

        MarchingCubes::output_functor pushTriangle = [&mutex, &marchingCubes, &vertexCounter,this]( Size primitiveIndex, const MarchingCubes::output_arguments& r_triangle ) -> void
        {
            std::scoped_lock lock(mutex);

            for ( const auto& r_edge : r_triangle )
            {
                // Compute vertex from edge midpoint
                auto vertex = ( marchingCubes.getVertex( primitiveIndex, r_edge.first ) + marchingCubes.getVertex( primitiveIndex, r_edge.second ) ) / 2.0;

                for ( MarchingPart::attribute_type component : vertex )
                    this->_p_attributes->push_back( component );

                this->_indices.push_back( vertexCounter++ );
            }
        };

        marchingCubes.setOutputFunctor( pushTriangle );
        marchingCubes.execute( std::make_shared<mp::ThreadPool>() );
    }
};




CSGObjectPtr makeTarget()
{
    auto p_unitSphere = CSGObjectPtr(
        new csg::boolean::Sphere<Dimension,CoordinateType>( {0.0,0.0,0.0}, 1.0 )
    );

    auto p_horizontalRod = CSGObjectPtr(
        new csg::boolean::Box<Dimension,CoordinateType>( {-2.0,-0.2,-0.2}, {4.0,0.4,0.4} )
    );

    auto p_cube = CSGObjectPtr(
        new csg::boolean::Cube<Dimension,CoordinateType>( {-0.9, -0.9, -0.9}, 1.8 )
    );

    auto p_geometry = p_unitSphere * p_cube - p_horizontalRod;

    auto p_target = std::make_shared<csg::Transform<Dimension,bool,CoordinateType>>( p_geometry );
    p_target->set( linalg::SpatialTransform<CoordinateType>::makeRotation(M_PI/4.0) );

    return p_target;
}




int main( int argc, char const* argv[] )
{
    // Construct target
    CSGObjectPtr p_target = makeTarget();

    // Graphics setup
    auto p_log = std::make_shared<utils::Logger>( OUTPUT_PATH / "StructuredMarchingCubes.log", true );

    auto p_context = gl::GLFWContextSingleton::get( p_log );

    auto p_window = p_context->newWindow( 1024, 768 );
    auto p_scene  = p_window->makeScene<ViewerScene>( "ViewerScene", *p_log );

    // March and add model
    auto timerID = p_scene->tic();

    Size resolution = 200;
    auto p_model = gl::PartPtr( new MarchingPart(
        p_target,
        { -1.0, -1.0, -1.0 },
        { resolution, resolution, resolution },
        2.0 / resolution
    ) );

    p_scene->toc( "Finished scanning", timerID );
    //p_model->writeSTL( OUTPUT_PATH / "csg2stl.stl" );

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
    auto p_controls = std::make_shared<gl::FlyCameraControls>(true);
    p_controls->bind( p_window, p_camera );

    p_controls->setMovementScale( 0.01 );
    p_controls->setRotationScale( 0.005 );
    p_controls->setZoomScale( 1.01 );

    // Event loop
    p_window->beginLoop();

    return 0;
}


} // namespace cie





int main( int argc, char const* argv[] )
{
    return cie::main( argc, argv );
}