#ifndef CIE_MESH_KERNEL_MARCHING_CUBES_HPP
#define CIE_MESH_KERNEL_MARCHING_CUBES_HPP

// --- CSG Includes ---
#include "CSG/packages/primitives/inc/Cube.hpp"
#include "CSG/packages/primitives/inc/Box.hpp"
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
typename PrimitiveType::point_type getVertexOnPrimitive( const PrimitiveType& r_primitive,
                                                         Size vertexIndex );

template <concepts::Box PrimitiveType>
typename PrimitiveType::point_type getVertexOnPrimitive( const PrimitiveType& r_primitive,
                                                         Size vertexIndex );
} // namespace detail



template < concepts::CSGObject TargetType,
           class PrimitiveType = csg::Cube<TargetType::dimension,typename TargetType::coordinate_type> >
requires (concepts::Cube<PrimitiveType> || concepts::Box<PrimitiveType>)
class UnstructuredMarchingCubes : public UnstructuredMarchingPrimitives<TargetType,PrimitiveType>
{
public:
    using typename UnstructuredMarchingPrimitives<TargetType,PrimitiveType>::point_type;
    using typename UnstructuredMarchingPrimitives<TargetType,PrimitiveType>::target_ptr;
    using typename UnstructuredMarchingPrimitives<TargetType,PrimitiveType>::output_functor;

    using primitive_container     = std::vector<PrimitiveType>;
    using primitive_container_ptr = std::shared_ptr<primitive_container>;

public:
    UnstructuredMarchingCubes( target_ptr p_target,
                               primitive_container_ptr p_primitives,
                               output_functor outputFunctor );

    point_type getVertexOnPrimitive( const PrimitiveType& r_primitive,
                                     Size vertexIndex ) const override;

protected:
    virtual Size primitiveVertexCount() const override;
};




template < concepts::CSGObject TargetType,
           class PrimitiveType = csg::Cube<TargetType::dimension,typename TargetType::coordinate_type> >
requires (concepts::Cube<PrimitiveType> || concepts::Box<PrimitiveType>)
class StructuredMarchingCubes : public StructuredMarchingPrimitives<TargetType,PrimitiveType>
{
public:
    using typename StructuredMarchingPrimitives<TargetType,PrimitiveType>::point_type;
    using typename StructuredMarchingPrimitives<TargetType,PrimitiveType>::target_ptr;
    using typename StructuredMarchingPrimitives<TargetType,PrimitiveType>::domain_specifier;
    using typename StructuredMarchingPrimitives<TargetType,PrimitiveType>::resolution_specifier;
    using typename StructuredMarchingPrimitives<TargetType,PrimitiveType>::output_functor;

public:
    StructuredMarchingCubes( target_ptr p_target,
                             const domain_specifier& r_domain,
                             const resolution_specifier& r_numberOfPoints,
                             output_functor outputFunctor );

    virtual Size getGlobalVertexIndex( Size primitiveIndex,
                                       Size vertexIndex ) const override;

    virtual Size primitiveVertexCount() const override;

    virtual Size numberOfRemainingPrimitives() const override;

    point_type getVertexOnPrimitive( const PrimitiveType& r_primitive,
                                     Size vertexIndex ) const override;

    private:
    /// Check whether the mesh domain can be discretized with cubes
    template <concepts::Cube T>
    void checkMesh() const;

    /// Do nothing
    template <concepts::Box T>
    void checkMesh() const;
};


} // namespace cie::mesh

#include "meshkernel/packages/marchingprimitives/impl/MarchingCubes_impl.hpp"

#endif