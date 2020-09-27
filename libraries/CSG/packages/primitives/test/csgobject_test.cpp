// --- External Includes ---
#include "catch.hpp"

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
        ValueType output;
        return output;
    }
};


TEST_CASE( "CSGObject", "[primitives]" )
{
    const Size dimension    = 2;

    REQUIRE_NOTHROW( CSGTestObject<dimension>() );

    CSGTestObject<dimension>                boolObject;
    CSGTestObject<dimension,Size>           sizeObject;
    CSGTestObject<dimension,Double>         doubleObject;
    CSGTestObject<dimension,DoubleVector>   vectorObject;

    std::array<Double,dimension>            doubleArray;
    std::vector<Double>                     doubleVector;
    std::deque<Double>                      doubleDeque;

    CHECK_NOTHROW( boolObject.evaluate(doubleArray) );
    CHECK_NOTHROW( sizeObject.evaluate(doubleArray) );
    CHECK_NOTHROW( doubleObject.evaluate(doubleArray) );
    CHECK_NOTHROW( vectorObject.evaluate(doubleArray) );

    doubleVector.resize(dimension);
    doubleDeque.resize(dimension);
    CHECK_NOTHROW( boolObject.evaluate(doubleVector) );
    CHECK_NOTHROW( boolObject.evaluate(doubleDeque) );
    CHECK_NOTHROW( sizeObject.evaluate(doubleVector) );
    CHECK_NOTHROW( sizeObject.evaluate(doubleDeque) );
    CHECK_NOTHROW( doubleObject.evaluate(doubleVector) );
    CHECK_NOTHROW( doubleObject.evaluate(doubleDeque) );
    CHECK_NOTHROW( vectorObject.evaluate(doubleVector) );
    CHECK_NOTHROW( vectorObject.evaluate(doubleDeque) );

    #ifdef CIE_ENABLE_OUT_OF_RANGE_TESTS
    doubleVector.resize(dimension+1);
    doubleDeque.resize(dimension+1);
    CHECK_THROWS( boolObject.evaluate(doubleVector) );
    CHECK_THROWS( boolObject.evaluate(doubleDeque) );
    CHECK_THROWS( sizeObject.evaluate(doubleVector) );
    CHECK_THROWS( sizeObject.evaluate(doubleDeque) );
    CHECK_THROWS( doubleObject.evaluate(doubleVector) );
    CHECK_THROWS( doubleObject.evaluate(doubleDeque) );
    CHECK_THROWS( vectorObject.evaluate(doubleVector) );
    CHECK_THROWS( vectorObject.evaluate(doubleDeque) );
    #endif


    REQUIRE_NOTHROW( CSGTestObject<dimension,Bool,Size>() );
    CSGTestObject<dimension,Bool,Size>  intCoordinateBoolObject;
    std::array<Size,dimension>          sizeArray;
    std::vector<Size>                   sizeVector;
    std::deque<Size>                    sizeDeque;

    sizeVector.resize(dimension);
    sizeDeque.resize(dimension);
    CHECK_NOTHROW( intCoordinateBoolObject.evaluate(sizeArray) );
    CHECK_NOTHROW( intCoordinateBoolObject.evaluate(sizeVector) );
    CHECK_NOTHROW( intCoordinateBoolObject.evaluate(sizeDeque) );

    #ifdef CIE_ENABLE_OUT_OF_RANGE_TESTS
    sizeVector.resize(dimension+1);
    sizeDeque.resize(dimension+1);
    CHECK_THROWS( intCoordinateBoolObject.evaluate(sizeVector) );
    CHECK_THROWS( intCoordinateBoolObject.evaluate(sizeDeque) );
    #endif
}


} // namespace cie::csg