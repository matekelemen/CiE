// --- CSG Includes ---
#include <csg/trees.hpp>

// --- Utility Includes ---
#include <cieutils/logging.hpp>
#include "cmake_variables.hpp"


namespace cie {


// --- TYPE ALIASES --- //

const Size Dimension = 3;
using CoordinateType = double;
using ValueType      = double;
using PointType      = typename csg::CSGTraits<Dimension,CoordinateType>::point_type;

using PrimitiveType  = csg::Box<Dimension,CoordinateType>;
using CellType       = csg::Cell<PrimitiveType>;
using NodeType       = csg::SpaceTreeNode<CellType,ValueType>;
using NodePtr        = std::shared_ptr<NodeType>;

using SamplerType    = csg::CartesianGridSampler<PrimitiveType>;
using SplitterType   = csg::WeightedSplitPolicy< NodeType::sample_point_iterator,
                                                 NodeType::value_iterator >;


// --- MAIN --- //

int main()
{
    utils::Logger log( OUTPUT_PATH / "space_tree_benchmark.log", true );

    const Size depth = 10;
    const Size numberOfPointsPerDimension = 5;

    log << "\x1b[38;2;0;255;0m";
    {
        auto localBlock = log.newBlock( "INFO" );
        localBlock << "Number of threads   : " + std::to_string( mp::ThreadPool::maxNumberOfThreads() );
        localBlock << "Number of dimensions: " + std::to_string( Dimension );
        localBlock << "Maximum depth       : " + std::to_string( depth );
        localBlock << "Sampling order      : " + std::to_string( numberOfPointsPerDimension );
    }
    log << "\x1b[0m";

    NodeType::target_function target = []( const PointType& r_point ) -> ValueType
    {
        return r_point[0]*r_point[0] + r_point[1]*r_point[1] + r_point[2]*r_point[2] - 1.0;
    };

    auto p_root = NodePtr( new NodeType(
        NodeType::sampler_ptr( new SamplerType(numberOfPointsPerDimension) ),
        NodeType::split_policy_ptr( new SplitterType ),
        0,
        PointType { -2.0, -2.0, -2.0 },
        PointType { 2.0, 2.0, 2.0 }
    ) );

    {
        auto localBlock = log.newBlock( "divide" );
        p_root->divide( target, depth );
    }

    return 0;
}


} // namespace cie




int main()
{
    return cie::main();
}