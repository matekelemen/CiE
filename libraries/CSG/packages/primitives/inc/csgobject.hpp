#ifndef CIE_CSG_CSG_OBJECT_HPP
#define CIE_CSG_CSG_OBJECT_HPP

// --- Utility Includes ---
#include "cieutils/packages/concepts/inc/container_concepts.hpp"
#include "cieutils/packages/types/inc/types.hpp"

// --- Internal Includes ---
#include "CSG/packages/primitives/inc/CSGTraits.hpp"

// --- STL Includes ---
#include <memory>
#include <array>

namespace cie::csg {

/// Interface for point-queriable objects
template <  Size N, 
            concepts::CopyConstructible ValueType = Bool,
            concepts::NumericType CoordinateType = Double   >
class CSGObject : public CSGTraits<N,CoordinateType>
{
public:
    typedef CSGObject                       abstract_base_type;
    typedef ValueType                       value_type;

    template <class ContainerType>
    ValueType evaluate( const ContainerType& point ) const
        requires concepts::ClassContainer<ContainerType,CoordinateType>;

    template <class ContainerType>
    ValueType evaluate( ContainerType&& point ) const
        requires concepts::ClassContainer<ContainerType,CoordinateType>;

protected:
    virtual ValueType at(const typename CSGObject::point_type& point) const = 0;
};


template <  Size N, 
            concepts::CopyConstructible ValueType,
            concepts::NumericType CoordinateType   >
using CSGObjectPtr = std::shared_ptr<CSGObject<N,ValueType,CoordinateType>>;



/* --- Convenience Functions --- */

namespace detail {

template < Size Dimension,
           concepts::NumericType CoordinateType >
typename CSGTraits<Dimension,CoordinateType>::point_type makeOrigin();

} // namespace detail


} // namespace cie::csg

#include "CSG/packages/primitives/impl/csgobject_impl.hpp"

#endif