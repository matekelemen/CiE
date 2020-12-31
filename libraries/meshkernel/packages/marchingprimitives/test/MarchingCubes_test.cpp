// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- CSG Includes ---
#include "CSG/packages/primitives/inc/Cube.hpp"
#include "CSG/packages/primitives/inc/Sphere.hpp"

// --- Internal Includes ---
#include "meshkernel/packages/marchingprimitives/inc/MarchingCubes.hpp"

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
    using TestType       = MarchingCubes<TargetType>;

    PointType meshOrigin { -2.0, -2.0, -2.0 };
    TestType::resolution_specifier numberOfCubes { 2, 2, 2 };
    CoordinateType edgeLength = 2.0;

    PointType origin     = csg::detail::makeOrigin<Dimension,CoordinateType>();
    auto p_unitSphere = TestType::target_ptr(
        new TargetType( origin, 1.0 )
    );

    std::vector<TestType::output_arguments> triangles;
    auto outputFunctor = [&triangles]( const TestType::output_arguments& r_triangle ) -> void
    {
        triangles.push_back( r_triangle );
    };

    TestType marchingCubes( p_unitSphere,
                            meshOrigin,
                            numberOfCubes,
                            edgeLength,
                            outputFunctor );

    marchingCubes.execute();

    for ( const auto& r_triangle : triangles )
    {
        for ( const auto& r_point : r_triangle )
        {
            for ( auto component : r_point )
                std::cout << component << ",";

            std::cout << std::endl;
        }

        std::cout << std::endl;
    }
}


} // namespace cie::mesh