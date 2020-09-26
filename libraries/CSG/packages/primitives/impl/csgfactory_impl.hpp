#ifndef CIE_CSG_CSGFACTORY_IMPL_HPP
#define CIE_CSG_CSGFACTORY_IMPL_HPP

namespace cie::csg {


template <  Size N, 
            concepts::CopyConstructible ValueType,
            concepts::NumericType CoordinateType   >
template <class CSGObjectType, class ...Args>
std::shared_ptr<CSGObjectType>
CSGFactory<N,ValueType,CoordinateType>::make( Args&&... args )
    requires concepts::DerivedFrom<CSGObjectType,CSGObject<N,ValueType,CoordinateType>>
{
    return std::make_shared<CSGObjectType>( std::forward<Args>(args)... );
}


} // namespace cie::csg

#endif