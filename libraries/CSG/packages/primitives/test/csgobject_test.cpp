// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Linalg Includes ---
#include "linalg/packages/types/inc/vectortypes.hpp"

// --- Internal Includes ---
#include "CSG/packages/primitives/inc/csgobject.hpp"

// --- STL Includes ---
#include <array>
#include <vector>
#include <deque>

namespace cie::csg {


template <  Size N, 
            class ValueType = Bool, 
            class CoordinateType = Double>
class CSGTestObject : public CSGObject<N,ValueType,CoordinateType>
{
protected:
    ValueType at(const typename CSGTestObject::point_type& point) const override
    {
        return ValueType();
    }
};


CIE_TEST_CASE( "CSGObject", "[primitives]" )
{
    CIE_TEST_CASE_INIT( "CSGObject" )

    const Size dimension    = 2;

    CIE_TEST_REQUIRE_NOTHROW( CSGTestObject<dimension>() );

    CSGTestObject<dimension>                boolObject;
    CSGTestObject<dimension,Size>           sizeObject;
    CSGTestObject<dimension,Double>         doubleObject;
    CSGTestObject<dimension,DoubleVector>   vectorObject;

    std::array<Double,dimension>            doubleArray;
    std::vector<Double>                     doubleVector;
    std::deque<Double>                      doubleDeque;

    CIE_TEST_CHECK_NOTHROW( boolObject.evaluate(doubleArray) );
    CIE_TEST_CHECK_NOTHROW( sizeObject.evaluate(doubleArray) );
    CIE_TEST_CHECK_NOTHROW( doubleObject.evaluate(doubleArray) );
    CIE_TEST_CHECK_NOTHROW( vectorObject.evaluate(doubleArray) );

    doubleVector.resize(dimension);
    doubleDeque.resize(dimension);
    CIE_TEST_CHECK_NOTHROW( boolObject.evaluate(doubleVector) );
    CIE_TEST_CHECK_NOTHROW( boolObject.evaluate(doubleDeque) );
    CIE_TEST_CHECK_NOTHROW( sizeObject.evaluate(doubleVector) );
    CIE_TEST_CHECK_NOTHROW( sizeObject.evaluate(doubleDeque) );
    CIE_TEST_CHECK_NOTHROW( doubleObject.evaluate(doubleVector) );
    CIE_TEST_CHECK_NOTHROW( doubleObject.evaluate(doubleDeque) );
    CIE_TEST_CHECK_NOTHROW( vectorObject.evaluate(doubleVector) );
    CIE_TEST_CHECK_NOTHROW( vectorObject.evaluate(doubleDeque) );

    #ifdef CIE_ENABLE_OUT_OF_RANGE_TESTS
    doubleVector.resize(dimension+1);
    doubleDeque.resize(dimension+1);
    CIE_TEST_CHECK_THROWS( boolObject.evaluate(doubleVector) );
    CIE_TEST_CHECK_THROWS( boolObject.evaluate(doubleDeque) );
    CIE_TEST_CHECK_THROWS( sizeObject.evaluate(doubleVector) );
    CIE_TEST_CHECK_THROWS( sizeObject.evaluate(doubleDeque) );
    CIE_TEST_CHECK_THROWS( doubleObject.evaluate(doubleVector) );
    CIE_TEST_CHECK_THROWS( doubleObject.evaluate(doubleDeque) );
    CIE_TEST_CHECK_THROWS( vectorObject.evaluate(doubleVector) );
    CIE_TEST_CHECK_THROWS( vectorObject.evaluate(doubleDeque) );
    #endif


    CIE_TEST_REQUIRE_NOTHROW( CSGTestObject<dimension,Bool,Size>() );
    CSGTestObject<dimension,Bool,Size>  intCoordinateBoolObject;
    std::array<Size,dimension>          sizeArray;
    std::vector<Size>                   sizeVector;
    std::deque<Size>                    sizeDeque;

    sizeVector.resize(dimension);
    sizeDeque.resize(dimension);
    CIE_TEST_CHECK_NOTHROW( intCoordinateBoolObject.evaluate(sizeArray) );
    CIE_TEST_CHECK_NOTHROW( intCoordinateBoolObject.evaluate(sizeVector) );
    CIE_TEST_CHECK_NOTHROW( intCoordinateBoolObject.evaluate(sizeDeque) );

    #ifdef CIE_ENABLE_OUT_OF_RANGE_TESTS
    sizeVector.resize(dimension+1);
    sizeDeque.resize(dimension+1);
    CIE_TEST_CHECK_THROWS( intCoordinateBoolObject.evaluate(sizeVector) );
    CIE_TEST_CHECK_THROWS( intCoordinateBoolObject.evaluate(sizeDeque) );
    #endif
}


} // namespace cie::csg