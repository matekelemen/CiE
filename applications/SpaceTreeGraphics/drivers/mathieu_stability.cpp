
// --- GL Includes ---
#include <ciegl/ciegl.hpp>

// --- CSG Includes ---
#include <csg/primitives.hpp>
#include <csg/trees.hpp>

// --- Mesh Includes ---
#include <meshkernel/marchingprimitives.hpp>

// --- Internal Includes ---
#include "hilbert_determinant.hpp"
#include "complex.hpp"

// --- STL Includes ---
#include <algorithm>
#include <mutex>


namespace cie {


// Typedefs
const Size Dimension    = 3;
using CoordinateType    = Double;
using ValueType         = Double;
using PointType         = typename csg::CSGTraits<Dimension,CoordinateType>::point_type;

using PrimitiveType    = csg::Cube<Dimension,CoordinateType>;
using CellType         = csg::CubeCell<PrimitiveType>;
using NodeType         = csg::SpaceTreeNode<CellType,ValueType>;
using NodePtr          = std::shared_ptr<NodeType>;

using SamplerType      = csg::CubeSampler<Dimension,CoordinateType>;
using SplitterType     = csg::MidPointSplitPolicy< typename NodeType::sample_point_iterator,
                                                   typename NodeType::value_iterator >;

using GeometryFunction = csg::TargetFunction<PointType,ValueType>;

using MarchingCubes    = mesh::UnstructuredMarchingCubes<csg::CSGObject<Dimension,Bool,CoordinateType>>;


// Target function
auto targetFunction = [](const PointType& r_parameters)
{
    double gamma = 0.1;
    double omega = 1.0;
    // double omegaCritical = 0.0;

    cie::Complex determinant = cie::hilbertDeterminant( 
                                            r_parameters[0],
                                            r_parameters[1],
                                            gamma,
                                            omega,
                                            r_parameters[2]/10.0,
                                            Dimension);
    double result = std::abs(determinant.real()) < std::abs(determinant.imag()) ? determinant.real() : determinant.imag();
    return result;
};


// Marching cubes -> triangulated part
class MarchingPart : public gl::Triangulated3DPart
{
public:
    MarchingPart( MarchingCubes::target_ptr p_target,
                  MarchingCubes::primitive_container_ptr p_cubes )
    {
        Size vertexCounter = 0;

        // Pull in linalg operator overloads from the global namespace
        using ::operator+;
        using ::operator/;

        MarchingCubes marchingCubes(
            p_target,
            p_cubes,
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


} // namespace cie


namespace cie::csg {

template <concepts::Cube Node>
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


template <concepts::Box Node>
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
    // Divide and collect leaf nodes
    auto p_cubes = MarchingCubes::primitive_container_ptr(
        new MarchingCubes::primitive_container
    );

    {
        auto p_root = csg::makeRoot<NodeType>();
        const Size depth = 8;

        #pragma omp parallel
        #pragma omp single
        p_root->divide(targetFunction,depth);

        auto pushLeafCube = [&p_root,&p_cubes]( NodeType* p_node ) -> bool
        {
            if ( p_node->isLeaf() && p_node->isBoundary() )
                p_cubes->emplace_back( *p_node );

            return true;
        };

        p_root->visit( pushLeafCube );
    }

    // Graphics setup
    auto p_context = gl::GLFWContextSingleton::get(
        4,                                          // <-- OpenGL version major
        5,                                          // <-- OpenGL version minor
        0,                                          // <-- Number of MSAA samples
        OUTPUT_PATH / "mathieu_stability_log.txt",  // <-- log file
        true                                        // <-- use console output
    );

    auto p_window = p_context->newWindow( 1024, 768 );
    auto p_scene = std::make_shared<gl::Triangulated3DPartScene>( *p_context,
                                                                  gl::Triangulated3DPartScene::part_container(),
                                                                  gl::CameraPtr(nullptr) );
    p_window->addScene( p_scene );

    // March and add part
    auto p_target = MarchingCubes::target_ptr( new csg::CSGObjectWrapper<Dimension,Bool,CoordinateType>( 
        []( const PointType& r_point ) -> bool { return targetFunction(r_point) > 0.0; }
     ));

    auto p_part = gl::PartPtr( new MarchingPart(
        p_target,
        p_cubes
    ) );

    p_scene->addPart( p_part );

    // Adjust camera
    auto p_camera = p_scene->camera();
    
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
    p_window->beginLoop();

    return 0;
}


} // namespace cie


int main()
{
    return cie::main();
}