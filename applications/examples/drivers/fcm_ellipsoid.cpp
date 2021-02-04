/** Compute element fill rations on an ellipsoidal domain
 * 
 * Crate a cartesian grid of hexahedral elements and generate a structured
 * set sample points on each one. Perform a point membership test on an
 * ellipsoid for all sample points of all elements, and compute an approximate
 * fill ratio based on the results.
 * 
 * Physical domain (ellipsoid):
 *  - center: { 1.0, 0.2, 0.1 }
 *  - radii : { 1.0, 0.2, 0.1 }
 * 
 * Cartesian mesh domain:
 *  - x in { 0.0, 1.0 }
 *  - y in { 0.0, 0.41 }
 *  - z in { 0.0, 0.21 }
 *  - resolution: 60 x 24 x 12
 *
 * Element sample points:
 *  - cartesian grid of 10 x 10 x 10 sample points
 * 
 * Write the approximate fill ratios to a csv file in a single column:
 *  - output file: "${OUTPUT_PATH}/fcm_ellipsoid.csv"
 */

// --- Utility Includes ---
#include <cieutils/logging.hpp>
#include <cieutils/concurrency.hpp>
#include "cmake_variables.hpp"

// --- CSG Includes ---
#include <csg/primitives.hpp>
#include <csg/trees.hpp>

// --- STL Includes ---
#include <fstream>


namespace cie {


// Typedefs
const Size Dimension = 3;
using CoordinateType = Double;
using ValueType      = Bool;
using Ellipsoid      = csg::boolean::Ellipsoid<Dimension,CoordinateType>;

using PointType      = Ellipsoid::point_type;

using NodePrimitive  = csg::Box<Dimension,CoordinateType>;
using CellType       = csg::Cell<NodePrimitive>;
using NodeType       = csg::SpaceTreeNode<CellType,ValueType>;
using NodePtr        = std::shared_ptr<NodeType>;

using SamplerType    = csg::CartesianGridSampler<NodePrimitive>;
using SplitterType   = csg::MidPointSplitPolicy< NodeType::sample_point_iterator,
                                                 NodeType::value_iterator >;

using TargetFunction = NodeType::target_function;


int main()
{
    utils::Logger log( OUTPUT_PATH / "fcm_ellipsoid.log", true );

    /* --- GEOMETRY AND DOMAIN --- */

    const Ellipsoid ellipsoid(
        { 1.0, 0.2, 0.1 },
        { 1.0, 0.2, 0.1 }
    );

    const PointType meshDomain {
        1.0,
        0.41,
        0.21
    };

    /* --- DISCRETIZATION --- */

    const std::vector<Size> numberOfElements {
        60,
        24,
        12
    };

    const Size numberOfPointsPerDimension = 10;

    /* --- SETUP --- */

    TargetFunction target = [&ellipsoid]( const auto& r_point )
    { return ellipsoid.evaluate(r_point); };

    std::vector<NodeType> nodes;
    nodes.reserve( numberOfElements[0] * numberOfElements[1] * numberOfElements[2] );

    const PointType elementLengths {
        meshDomain[0] / numberOfElements[0],
        meshDomain[1] / numberOfElements[1],
        meshDomain[2] / numberOfElements[2]
    };

    auto p_sampler = NodeType::sampler_ptr( new SamplerType(numberOfPointsPerDimension) );
    auto p_splitter = NodeType::split_policy_ptr( new SplitterType );

    /* --- SCAN --- */

    {
        mp::ThreadPool pool;
        auto localBlock = log.newBlock( "scan" );

        for ( Size i=0; i<numberOfElements[0]; ++i )
            for ( Size j=0; j<numberOfElements[1]; ++j )
                for ( Size k=0; k<numberOfElements[2]; ++k )
                {
                    auto* p_node = &nodes.emplace_back(
                        p_sampler,
                        p_splitter,
                        0,
                        PointType { i*elementLengths[0], j*elementLengths[1], k*elementLengths[2] },
                        elementLengths
                    );

                    pool.queueJob(
                        [p_node, target]() -> void
                        { p_node->evaluate( target ); }
                    );
                }

        pool.terminate();
    }

    /* --- COLLECT RESULTS --- */

    const double numberOfSamplesPerNode = nodes.back().values().size();

    std::ofstream file( OUTPUT_PATH / "fcm_ellipsoid.csv" );

    for ( const auto& r_node : nodes )
    {
        Size insideCounter = 0;
        for ( auto value : r_node.values() )
            if ( value )
                ++insideCounter;
        
        file << insideCounter / numberOfSamplesPerNode << std::endl;
    }

    return 0;
}


} // namespace cie




int main()
{
    return cie::main();
}