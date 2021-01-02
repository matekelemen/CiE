// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- CSG Includes ---
#include "CSG/packages/primitives/inc/Cube.hpp"
#include "CSG/packages/primitives/inc/Sphere.hpp"

// --- Internal Includes ---
#include "meshkernel/packages/marchingprimitives/inc/MarchingCubes.hpp"
#include "meshkernel/packages/structured/inc/cartesianmesh.hpp"

// --- STL Includes ---
#include <vector>


namespace cie::mesh {


CIE_TEST_CASE( "MarchingCubes", "[marchingprimitives]" )
{
    CIE_TEST_CASE_INIT( "MarchingCubes" )

    const Size Dimension = 3;
    using CoordinateType = double;
    using PointType      = MeshTraits<Dimension,CoordinateType>::point_type;
    using TargetType     = csg::boolean::Sphere<Dimension,CoordinateType>;

    {
        CIE_TEST_CASE_INIT( "UnstructuredMarchingCubes" )
        using TestType = UnstructuredMarchingCubes<TargetType>;

        PointType meshOrigin { -2.0, -2.0, -2.0 };
        TestType::resolution_specifier numberOfCubes { 2, 2, 2 };
        CoordinateType edgeLength = 2.0;

        PointType origin     = csg::detail::makeOrigin<Dimension,CoordinateType>();
        auto p_unitSphere = TestType::target_ptr(
            new TargetType( origin, 1.0 )
        );

        std::vector<TestType::output_arguments> outputs;
        auto outputFunctor = [&outputs]( Size primitiveIndex, const TestType::output_arguments& r_edges ) -> void
        {};

        auto p_primitives = TestType::primitive_container_ptr(
            new TestType::primitive_container
        );

        makeCartesianMesh<TestType::primitive_type>( numberOfCubes,
                                                    edgeLength,
                                                    meshOrigin,
                                                    *p_primitives );

        TestType marchingCubes( p_unitSphere,
                                p_primitives,
                                outputFunctor );

        marchingCubes.execute();

//      for ( const auto& r_triangle : outputs )
//      {
//          for ( const auto& r_edge : r_triangle )
//              std::cout << r_edge.first << " " << r_edge.second << std::endl;
//  
//          std::cout << std::endl;
//      }
    }

    {
        CIE_TEST_CASE_INIT( "StructuredMarchingCubes" )

        using TestType = StructuredMarchingCubes<TargetType>;

        TestType::domain_specifier domain {{ {-2.0,2.0}, {-2.0,2.0}, {-2.0,2.0} }};
        TestType::resolution_specifier numberOfPoints { 3, 3, 3 };

        PointType origin     = csg::detail::makeOrigin<Dimension,CoordinateType>();
        auto p_unitSphere = TestType::target_ptr(
            new TargetType( origin, 1.0 )
        );

        std::vector<TestType::output_arguments> outputs;
        auto outputFunctor = [&outputs]( Size primitiveIndex, const TestType::output_arguments& r_edges ) -> void
        {};

        TestType( p_unitSphere,
                  domain,
                  numberOfPoints,
                  outputFunctor ).execute();
    }
}


} // namespace cie::mesh