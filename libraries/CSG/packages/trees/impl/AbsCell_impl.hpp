#ifndef CIE_CSG_TREES_ABS_CELL_IMPL_HPP
#define CIE_CSG_TREES_ABS_CELL_IMPL_HPP


namespace cie::csg {

template <concepts::Primitive PrimitiveType>
template <class ...Args>
AbsCell<PrimitiveType>::AbsCell( Args&&... args ) :
    PrimitiveType( std::forward<Args>(args) ... )
{
}



template <concepts::Primitive PrimitiveType>
inline typename AbsCell<PrimitiveType>::primitive_constructor_container_ptr
AbsCell<PrimitiveType>::split( const typename AbsCell<PrimitiveType>::point_type& point )
{
    return this->split_internal(point);
}


} // namespace cie::csg

#endif