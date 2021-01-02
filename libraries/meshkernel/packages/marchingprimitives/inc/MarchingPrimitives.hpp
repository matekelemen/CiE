#ifndef CIE_MESH_KERNEL_MARCHING_PRIMITIVES_HPP
#define CIE_MESH_KERNEL_MARCHING_PRIMITIVES_HPP

// --- Internal Includes ---
#include "meshkernel/packages/marchingprimitives/inc/AbsMarchingPrimitives.hpp"

namespace cie::mesh {


template <concepts::CSGObject TargetType, concepts::Primitive PrimitiveType>
class MarchingPrimitives : public AbsMarchingPrimitives<TargetType>
{
public:
    using primitive_type = PrimitiveType;

public:
    template <class ...Args>
    MarchingPrimitives( Args&&... args );

    virtual typename MarchingPrimitives<TargetType,PrimitiveType>::point_type getVertexOnPrimitive( const primitive_type& r_primitive, Size vertexIndex ) const = 0;
};



/// MarchingPrimitives that scans a container of primitives
template <concepts::CSGObject TargetType, concepts::Primitive PrimitiveType>
class UnstructuredMarchingPrimitives : public MarchingPrimitives<TargetType,PrimitiveType>
{
public:
    using primitive_container     = std::vector<typename UnstructuredMarchingPrimitives<TargetType,PrimitiveType>::primitive_type>;
    using primitive_container_ptr = std::shared_ptr<primitive_container>;

public:
    UnstructuredMarchingPrimitives( typename UnstructuredMarchingPrimitives<TargetType,PrimitiveType>::target_ptr p_target,
                                    const typename UnstructuredMarchingPrimitives<TargetType,PrimitiveType>::edge_table& r_edgeTable,
                                    const typename UnstructuredMarchingPrimitives<TargetType,PrimitiveType>::connectivity_table& r_connectivityTable,
                                    typename UnstructuredMarchingPrimitives<TargetType,PrimitiveType>::output_functor outputFunctor,
                                    primitive_container_ptr p_primitives );

    virtual typename UnstructuredMarchingPrimitives<TargetType,PrimitiveType>::point_type getVertex( Size primitiveIndex,
                                                                                                     Size vertexIndex ) override;

    virtual Size numberOfRemainingPrimitives() const override;

    const primitive_container& primitives() const;

public:
    primitive_container_ptr _p_primitives;
};



/// MarchingPrimitives that scans a cartesian mesh of primitives
template <concepts::CSGObject TargetType, concepts::Primitive PrimitiveType>
class StructuredMarchingPrimitives : public MarchingPrimitives<TargetType,PrimitiveType>
{
public:
    
};


} // namespace cie::mesh

#include "meshkernel/packages/marchingprimitives/impl/MarchingPrimitives_impl.hpp"

#endif