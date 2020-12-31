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

public:
    MarchingCubes( typename MarchingCubes<TargetType>::target_ptr p_target,
                   primitive_container_ptr p_primitives,
                   typename MarchingCubes<TargetType>::output_functor outputFunctor );

    MarchingCubes( typename MarchingCubes<TargetType>::target_ptr p_target,
                   const MarchingCubes<TargetType>::point_type& r_origin,
                   const typename MarchingCubes<TargetType>::resolution_specifier& r_numberOfPrimitives,
                   MarchingCubes<TargetType>::coordinate_type edgeLength,
                   typename MarchingCubes<TargetType>::output_functor outputFunctor );

protected:
    virtual bool getNextPrimitive( typename MarchingCubes<TargetType>::point_container& r_vertices ) override;

protected:
    primitive_container_ptr _p_primitives;
    Size                    _primitiveIndex;
};


} // namespace cie::mesh

#include "meshkernel/packages/marchingprimitives/impl/MarchingCubes_impl.hpp"

#endif