#ifndef CIE_MESH_KERNEL_MARCHING_CUBES_HPP
#define CIE_MESH_KERNEL_MARCHING_CUBES_HPP

// --- CSG Includes ---
#include "CSG/packages/primitives/inc/Cube.hpp"

// --- Internal Includes ---
#include "meshkernel/packages/marchingprimitives/inc/MarchingPrimitives.hpp"

// --- STL Includes ---
#include <array>
#include <utility>
#include <functional>


namespace cie::mesh {


namespace detail {
template <concepts::Cube PrimitiveType>
typename PrimitiveType::point_type getVertexOnCube( const PrimitiveType& r_primitive,
                                                    Size vertexIndex );
} // namespace detail



template <concepts::CSGObject TargetType>
class UnstructuredMarchingCubes : public UnstructuredMarchingPrimitives<TargetType,csg::Cube<TargetType::dimension,typename TargetType::coordinate_type>>
{
public:
    using primitive_container     = std::vector<typename UnstructuredMarchingCubes<TargetType>::primitive_type>;
    using primitive_container_ptr = std::shared_ptr<primitive_container>;

public:
    UnstructuredMarchingCubes( typename UnstructuredMarchingCubes<TargetType>::target_ptr p_target,
                               primitive_container_ptr p_primitives,
                               typename UnstructuredMarchingCubes<TargetType>::output_functor outputFunctor );

    typename UnstructuredMarchingCubes<TargetType>::point_type getVertexOnPrimitive( const typename UnstructuredMarchingCubes<TargetType>::primitive_type& r_primitive,
                                                                                     Size vertexIndex ) const override;

protected:
    virtual Size primitiveVertexCount() const override;
};


} // namespace cie::mesh

#include "meshkernel/packages/marchingprimitives/impl/MarchingCubes_impl.hpp"

#endif