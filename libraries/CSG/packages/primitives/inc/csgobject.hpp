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
template < Size N, 
           concepts::CopyConstructible ValueType,
           concepts::NumericType CoordinateType >
class CSGObject : public CSGTraits<N,CoordinateType>
{
public:
    using typename CSGTraits<N,CoordinateType>::point_type;

    using abstract_base_type = CSGObject<N,ValueType,CoordinateType>;
    using value_type         = ValueType;

public:
    virtual ~CSGObject() {}

    ValueType evaluate( const point_type& r_point ) const;

    template <class ContainerType>
    ValueType evaluate( const ContainerType& point ) const
        requires concepts::ClassContainer<ContainerType,CoordinateType>;

    template <class ContainerType>
    ValueType evaluate( ContainerType&& point ) const
        requires concepts::ClassContainer<ContainerType,CoordinateType>;

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