#ifndef CIE_GL_PLOT_2_MARCHING_CUBES_HPP
#define CIE_GL_PLOT_2_MARCHING_CUBES_HPP

// --- Mesh Includes ---
#include "meshkernel/packages/marchingprimitives/inc/MarchingCubes.hpp"
#include "meshkernel/packages/marchingprimitives/inc/concepts.hpp"

// --- Internal Includes ---
#include "ciegl/packages/plot/inc/Plot2.hpp"


namespace cie::gl {


template <concepts::MarchingPrimitives ObjectType>
requires concepts::Cube<typename ObjectType::primitive_type>
struct Plot2<ObjectType> : public AbsPlot2<std::true_type>
{
    Plot2( ObjectType& r_marchingCubes );
};


template <concepts::MarchingPrimitives ObjectType>
requires concepts::Cube<typename ObjectType::primitive_type>
void plot2( ObjectType& r_marchingCubes );


} // namespace cie::gl

#include "ciegl/packages/plot/impl/Plot2_MarchingCubes_impl.hpp"

#endif