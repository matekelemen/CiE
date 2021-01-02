#ifndef CIE_MESH_KERNEL_MARCHING_PRIMITIVES_IMPL_HPP
#define CIE_MESH_KERNEL_MARCHING_PRIMITIVES_IMPL_HPP


namespace cie::mesh {


template <concepts::CSGObject TargetType, concepts::Primitive PrimitiveType>
template <class ...Args>
MarchingPrimitives<TargetType,PrimitiveType>::MarchingPrimitives( Args&&... args ) :
    AbsMarchingPrimitives<TargetType>( std::forward<Args>(args)... )
{
}


template <concepts::CSGObject TargetType, concepts::Primitive PrimitiveType>
UnstructuredMarchingPrimitives<TargetType,PrimitiveType>::UnstructuredMarchingPrimitives( typename UnstructuredMarchingPrimitives<TargetType,PrimitiveType>::target_ptr p_target,
                                                                                          const typename UnstructuredMarchingPrimitives<TargetType,PrimitiveType>::edge_table& r_edgeTable,
                                                                                          const typename UnstructuredMarchingPrimitives<TargetType,PrimitiveType>::connectivity_table& r_connectivityTable,
                                                                                          typename UnstructuredMarchingPrimitives<TargetType,PrimitiveType>::output_functor outputFunctor,
                                                                                          typename UnstructuredMarchingPrimitives<TargetType,PrimitiveType>::primitive_container_ptr p_primitives ) :
    MarchingPrimitives<TargetType,PrimitiveType>( p_target,
                                                  r_edgeTable,
                                                  r_connectivityTable,
                                                  outputFunctor ),
    _p_primitives( p_primitives )
{
}


template <concepts::CSGObject TargetType, concepts::Primitive PrimitiveType>
inline typename UnstructuredMarchingPrimitives<TargetType,PrimitiveType>::point_type
UnstructuredMarchingPrimitives<TargetType,PrimitiveType>::getVertex( Size primitiveIndex,
                                                                     Size vertexIndex )
{
    return this->getVertexOnPrimitive( this->_p_primitives->at( primitiveIndex ),
                                       vertexIndex );
}


template <concepts::CSGObject TargetType, concepts::Primitive PrimitiveType>
inline Size
UnstructuredMarchingPrimitives<TargetType,PrimitiveType>::numberOfRemainingPrimitives() const
{
    return this->_p_primitives->size();
}


template <concepts::CSGObject TargetType, concepts::Primitive PrimitiveType>
inline const UnstructuredMarchingPrimitives<TargetType,PrimitiveType>::primitive_container&
UnstructuredMarchingPrimitives<TargetType,PrimitiveType>::primitives() const
{
    return *this->_p_primitives;
}


} // namespace cie::mesh


#endif