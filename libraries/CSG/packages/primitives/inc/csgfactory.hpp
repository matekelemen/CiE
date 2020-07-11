#ifndef CIE_CSG_CSGFACTORY_HPP
#define CIE_CSG_CSGFACTORY_HPP

// --- Internal Includes ---
#include "csgobject.hpp"

// --- Utility Includes ---
#include <cieutils/concepts.hpp>

// --- STL Includes ---
#include <memory>

namespace cie{
namespace csg {


template <  Size N, 
            concepts::CopyConstructible ValueType = Bool,
            concepts::NumericType CoordinateType = Double   >
struct CSGFactory
{
    template <class CSGObjectType, class ...Args>
    static std::shared_ptr<CSGObjectType> make( Args&&... args )
        requires concepts::DerivedFrom<CSGObjectType,CSGObject<N,ValueType,CoordinateType>>;
};


}
}

#include "../impl/csgfactory_impl.hpp"

#endif