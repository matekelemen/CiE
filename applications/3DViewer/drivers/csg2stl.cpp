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
using MarchingCubes       = mesh::UnstructuredMarchingCubes<CSGObject>;



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

        auto p_primitives = MarchingCubes::primitive_container_ptr(
            new MarchingCubes::primitive_container
        );
        mesh::makeCartesianMesh<MarchingCubes::primitive_type>(
            r_numberOfCubes,
            edgeLength,
            r_origin,
            *p_primitives
        );

        MarchingCubes marchingCubes(
            p_target,
            p_primitives,
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

                for ( MarchingPart::data_type component : vertex )
                    this->_data.push_back( component );

                this->_indices.push_back( vertexCounter++ );
            }
        };

        marchingCubes.setOutputFunctor( pushTriangle );
        marchingCubes.execute();
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

    return p_unitSphere * p_cube - p_horizontalRod;
}




int main( int argc, char const* argv[] )
{
    // Construct target
    CSGObjectPtr p_target = makeTarget();

    // Graphics setup
    auto p_context = gl::GLFWContextSingleton::get(
        OUTPUT_PATH / "csg2stl.log",                    // <-- log file
        true                                            // use console output
    );

    auto p_window = p_context->newWindow( 1024, 768 );
    auto p_scene  = std::make_shared<gl::Triangulated3DPartScene>( *p_context,
                                                                   gl::Triangulated3DPartScene::part_container(),
                                                                   gl::CameraPtr(nullptr) );
    p_window->addScene( p_scene);

    // March and add model
    auto timerID = p_scene->tic();

    Size resolution = 200;
    auto p_part = gl::PartPtr( new MarchingPart(
        p_target,
        { -1.0, -1.0, -1.0 },
        { resolution, resolution, resolution },
        2.0 / resolution
    ) );

    p_scene->toc( "Finished scanning", timerID );
    //p_model->writeSTL( OUTPUT_PATH / "csg2stl.stl" );

    p_scene->addPart( p_part );

    // Set initial camera pose
    auto p_camera = p_scene->camera();
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