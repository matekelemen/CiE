#ifndef CIE_CSG_OBJECT_WRAPPER_HPP
#define CIE_CSG_OBJECT_WRAPPER_HPP

// --- Internal Includes ---
#include "CSG/packages/primitives/inc/csgobject.hpp"

// --- STL Includes ---
#include <functional>


namespace cie::csg {


template < Size Dimension,
           concepts::CopyConstructible ValueType,
           concepts::NumericType CoordinateType >
class CSGObjectWrapper : public CSGObject<Dimension,ValueType,CoordinateType>
{
public:
    using typename CSGObject<Dimension,ValueType,CoordinateType>::point_type;
    using functor_type = std::function<ValueType(const point_type&)>;

public:
    CSGObjectWrapper( functor_type functor );
    CSGObjectWrapper( const CSGObjectWrapper<Dimension,ValueType,CoordinateType>& r_rhs ) = default;
    CSGObjectWrapper( CSGObjectWrapper<Dimension,ValueType,CoordinateType>&& r_rhs ) = default;
    CSGObjectWrapper<Dimension,ValueType,CoordinateType>& operator=( const CSGObjectWrapper<Dimension,ValueType,CoordinateType>& r_rhs ) = default;

    virtual ValueType at( const point_type& r_point ) const override;

private:
    CSGObjectWrapper() = delete;

protected:
    functor_type _functor;
};


} // namespace cie::csg

#include "CSG/packages/primitives/impl/CSGObjectWrapper_impl.hpp"

#endif