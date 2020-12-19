// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "CSG/packages/primitives/inc/CSGTraits.hpp"


namespace cie::csg {


namespace detail {
template <concepts::CSGObject T>
bool isCSGObject( const T& reference ) { return true; }

template <class T>
requires (!concepts::CSGObject<T>)
bool isCSGObject( const T& reference ) { return false; }
} // namespace detail


CIE_TEST_CASE( "CSGTraits", "[primitives]" )
{
    CIE_TEST_CASE_INIT( "CSGTraits" )

    using RawTraits = CSGTraits<1,Double>;
    RawTraits rawTraits;
    CIE_TEST_CHECK( detail::isCSGObject(rawTraits) );

    using DeducedTraits = Traits<RawTraits>;
    DeducedTraits deducedTraits;
    CIE_TEST_CHECK( detail::isCSGObject(deducedTraits) );

    using PointTraits = Traits<RawTraits::point_type>;
    PointTraits pointTraits;
    CIE_TEST_CHECK( detail::isCSGObject(pointTraits) );
}


} // namespace cie::csg