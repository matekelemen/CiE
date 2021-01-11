// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- CSG Includes ---
#include "CSG/packages/primitives/inc/Sphere.hpp"

// --- Internal Includes ---
#include "ciegl/packages/plot/inc/Plot2_MarchingCubes.hpp"

// --- STL Includes ---
#include <chrono>
#include <thread>


namespace cie::gl {


CIE_TEST_CASE( "Plot2 - MarchingCubes", "[plot]" )
{
    CIE_TEST_CASE_INIT( "Plot2 - MarchingCubes" )

    // Construct marching cubes
    const Size Dimension = 2;
    using CoordinateType = Double;
    using TargetType     = csg::CSGObject<Dimension,Bool,CoordinateType>;
    using MarchingCubes  = mesh::StructuredMarchingCubes<TargetType>;

    auto p_target = MarchingCubes::target_ptr(
        new csg::boolean::Sphere<Dimension,CoordinateType>( {0.0,0.0}, 1.0 )
    );

    MarchingCubes::domain_specifier domain {{ {-1.0,1.0}, {-1.0,1.0} }};
    MarchingCubes::resolution_specifier numberOfPoints { 25, 25 };

    MarchingCubes marchingCubes( p_target,
                                 domain,
                                 numberOfPoints,
                                 nullptr );

    Plot2<MarchingCubes> plot( marchingCubes );
    plot.update();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    //plot2( marchingCubes );
}


} // namespace cie::gl