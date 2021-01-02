#ifndef CIE_MESH_KERNEL_MARCHING_CUBES_HPP
#define CIE_MESH_KERNEL_MARCHING_CUBES_HPP

// --- CSG Includes ---
#include "CSG/packages/primitives/inc/Cube.hpp"
#include "CSG/packages/trees/inc/indexconverter.hpp"

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




template <concepts::CSGObject TargetType>
class StructuredMarchingCubes : public StructuredMarchingPrimitives<TargetType,csg::Cube<TargetType::dimension,typename TargetType::coordinate_type>>
{
public:
    using typename StructuredMarchingPrimitives<TargetType,typename StructuredMarchingCubes<TargetType>::primitive_type>::primitive_type;
    using typename StructuredMarchingPrimitives<TargetType,primitive_type>::target_ptr;
    using typename StructuredMarchingPrimitives<TargetType,primitive_type>::domain_specifier;
    using typename StructuredMarchingPrimitives<TargetType,primitive_type>::resolution_specifier;
    using typename StructuredMarchingPrimitives<TargetType,primitive_type>::output_functor;

public:
    StructuredMarchingCubes( target_ptr p_target,
                             const domain_specifier& r_domain,
                             const resolution_specifier& r_numberOfPoints,
                             output_functor outputFunctor );

    virtual Size getGlobalVertexIndex( Size primitiveIndex,
                                       Size vertexIndex ) const override;

    virtual Size primitiveVertexCount() const override;

    virtual Size numberOfRemainingPrimitives() const override;

    typename StructuredMarchingCubes<TargetType>::point_type getVertexOnPrimitive( const typename StructuredMarchingCubes<TargetType>::primitive_type& r_primitive,
                                                                                   Size vertexIndex ) const override;
};


} // namespace cie::mesh

#include "meshkernel/packages/marchingprimitives/impl/MarchingCubes_impl.hpp"

#endif