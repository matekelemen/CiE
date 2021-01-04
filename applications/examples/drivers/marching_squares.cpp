// --- Graphics Includes ---
#include <ciegl/plot.hpp>

// --- Mesh Includes ---
#include <meshkernel/marchingprimitives.hpp>

// --- CSG Includes ---
#include <csg/primitives.hpp>
#include <csg/operators.hpp>


namespace cie {



/* --- TypeDefs --- */

const Size Dimension  = 2;
using CoordinateType  = Double;

using CSGTarget       = csg::CSGObject<Dimension,Bool,CoordinateType>;
using MarchingSquares = mesh::StructuredMarchingCubes<CSGTarget>;

using Cube            = csg::boolean::Cube<Dimension,CoordinateType>;
using Box             = csg::boolean::Box<Dimension,CoordinateType>;
using Sphere          = csg::boolean::Sphere<Dimension,CoordinateType>;

using csg::operator+;
using csg::operator-;
using csg::operator*;


/* --- Target Geometry --- */

MarchingSquares::target_ptr makeTargetGeometry()
{
    auto p_horizontalBar = MarchingSquares::target_ptr(
        new Box( {-1.75,-0.3}, {3.5,0.6} )
    );

    auto p_unitCircle    = MarchingSquares::target_ptr(
        new Sphere( {0.0,0.0}, 1.0 )
    );

    auto p_cuttingCube   = MarchingSquares::target_ptr(
        new Cube( {-0.9,-0.9}, 1.8 )
    );

    auto p_innerCube     = MarchingSquares::target_ptr(
        new Cube( {-0.2,-0.2}, 0.4 )
    );

    return p_horizontalBar + p_unitCircle * p_cuttingCube - p_innerCube;
}


/* --- Driver --- */

int main( int argc, char const* argv[] )
{
    // Marching squares setup
    MarchingSquares::domain_specifier domain                {{ {-2.0, 2.0}, {-1.5, 1.5} }};
    MarchingSquares::resolution_specifier numberOfPoints    { 401, 301 };

    MarchingSquares marchingSquares( makeTargetGeometry(),
                                     domain,
                                     numberOfPoints,
                                     nullptr );

    gl::plot2( marchingSquares );

    return 0;
}


} // namespace cie



int main( int argc, char const* argv[] )
{
    return cie::main( argc, argv );
}