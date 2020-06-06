#ifndef CIE_CSG_CSG_OBJECT_IMPL_HPP
#define CIE_CSG_CSG_OBJECT_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/macros.hpp"

namespace cie {
namespace csg {


template <  Size N, 
            concepts::CopyConstructible ValueType,
            concepts::NumericType CoordinateType    >
template <class ContainerType>
ValueType
CSGObject<N,ValueType,CoordinateType>::evaluate( const ContainerType& point ) const
    requires concepts::ClassContainer<ContainerType,CoordinateType>
{
    CIE_OUT_OF_RANGE_TEST( point.size() == N, "CSGObject" )

    CSGObject<N,ValueType,CoordinateType>::point_type internalPoint;
    std::copy(  point.begin(),
                point.end(),
                internalPoint.begin() );
    return this->operator()(internalPoint);
}


template <  Size N, 
            concepts::CopyConstructible ValueType,
            concepts::NumericType CoordinateType    >
template <class ContainerType>
ValueType
CSGObject<N,ValueType,CoordinateType>::evaluate( ContainerType&& point ) const
    requires concepts::ClassContainer<ContainerType,CoordinateType>
{
    CIE_OUT_OF_RANGE_TEST( point.size() == N, "CSGObject" )
    
    CSGObject<N,ValueType,CoordinateType>::point_type internalPoint;
    std::move(  point.begin(),
                point.end(),
                internalPoint.begin() );
    return this->operator()(internalPoint);
}


}
}

#endif