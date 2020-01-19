#include "catch.hpp"
#include "../inc/csgobject.hpp"

namespace cie {
namespace csg {


template <size_t N>
struct CSGTestObject : public CSGObject<N>
{
    double operator()(const DoubleArray<N>& point) const
    {
        return 1.0;
    }
};


TEST_CASE("CSGObject")
{
    const size_t dimension    = 2;

    CHECK_NOTHROW( CSGTestObject<dimension>() );
}


}
}