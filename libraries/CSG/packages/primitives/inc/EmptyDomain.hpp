#ifndef CIE_CSG_EMPTY_DOMAIN_HPP
#define CIE_CSG_EMPTY_DOMAIN_HPP

// --- Internal Includes ---
#include "CSG/packages/primitives/inc/csgobject.hpp"


namespace cie::csg {


template <Size Dimension, concepts::NumericType CoordinateType>
class EmptyDomain : public CSGObject<Dimension,Bool,CoordinateType>
{
public:
    virtual Bool at( const typename EmptyDomain<Dimension,CoordinateType>::point_type& r_point ) const override;
};


} // namespace cie::csg

#include "CSG/packages/primitives/impl/EmptyDomain_impl.hpp"

#endif