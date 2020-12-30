#ifndef CIE_MESH_KERNEL_MARCHING_CUBES_HPP
#define CIE_MESH_KERNEL_MARCHING_CUBES_HPP

// --- CSG Includes ---
#include "CSG/packages/primitives/inc/Cube.hpp"

// --- Internal Includes ---
#include "meshkernel/packages/marchingprimitives/inc/MarchingPrimitives.hpp"

// --- STL Includes ---
#include <array>
#include <utility>


namespace cie::mesh {


template <concepts::CSGObject TargetType>
class MarchingCubes : public MarchingPrimitives<TargetType>
{
public:
    using primitive_type          = csg::Cube<3,typename MarchingCubes<TargetType>::coordinate_type>;
    using primitive_container     = std::vector<primitive_type>;
    using primitive_container_ptr = std::shared_ptr<primitive_container>;

    using domain_specifier        = std::array<std::pair<CoordinateType,CoordinateType>,MarchingCubes<TargetType>::dimension>;
    using resolution_specifier    = std::array<Size,MarchingCubes<TargetType>::dimension>;

public:
    MarchingCubes( typename MarchingCubes<TargetType>::target_ptr p_target,
                   primitive_container_ptr p_primitives,
                   typename MarchingCubes<TargetType>::output_functor outputFunctor );

    MarchingCubes( typename MarchingCubes<TargetType>::target_ptr p_target,
                   const domain_specifier& r_domain,
                   const resolution_specifier& r_resolution,
                   typename MarchingCubes<TargetType>::output_functor outputFunctor );

protected:
    virtual bool getNextPrimitive( typename MarchingCubes<CoordinateType>::point_container& r_vertices ) override;

protected:
    primitive_container_ptr _p_primitives;
};


} // namespace cie::mesh

#include "meshkernel/packages/marchingprimitives/impl/MarchingCubes_impl.hpp"

#endif