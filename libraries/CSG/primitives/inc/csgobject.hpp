#ifndef CIE_CSG_CSG_OBJECT_HPP
#define CIE_CSG_CSG_OBJECT_HPP

// --- Utility Includes ---
#include "cieutils/types.hpp"
#include "cieutils/concepts.hpp"

namespace cie {
namespace csg {


template <  Size N, 
            concepts::CopyConstructible ValueType = Bool,
            concepts::NumericType CoordinateType = Double   >
class CSGObject
{
public:
    typedef ValueType                       value_type;
    typedef CoordinateType                  coordinate_type;
    typedef std::array<CoordinateType,N>    point_type;
    static const Size                       dimension = N;

    template <class ContainerType>
    ValueType evaluate( const ContainerType& point ) const
        requires concepts::ClassContainer<ContainerType,CoordinateType>;

    template <class ContainerType>
    ValueType evaluate( ContainerType&& point ) const
        requires concepts::ClassContainer<ContainerType,CoordinateType>;

protected:
    virtual ValueType operator()(const point_type& point) const = 0;
};


}
}

#include "csgobject_impl.hpp"

#endif