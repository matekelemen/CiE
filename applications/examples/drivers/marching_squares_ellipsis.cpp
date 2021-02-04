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

using csg::operator+;
using csg::operator-;
using csg::operator*;


/* --- Target Geometry --- */

MarchingSquares::target_ptr makeTargetGeometry()
{
    return MarchingSquares::target_ptr(
        new cie::csg::boolean::Ellipsoid<Dimension,CoordinateType>(
            { 1.0, 1.0 },
            { 2.0, 0.5 }
        )
    );
}


/* --- Driver --- */

int main( int argc, char const* argv[] )
{
    // Marching squares setup
    MarchingSquares::domain_specifier domain                {{ {-1.0, 3.0}, {0.0, 2.0} }};
    MarchingSquares::resolution_specifier numberOfPoints    { 401, 201 };

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