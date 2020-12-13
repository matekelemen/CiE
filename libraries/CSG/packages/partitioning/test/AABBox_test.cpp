// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"

// --- Internal Includes ---
#include "CSG/packages/partitioning/inc/AABBox.hpp"


namespace cie::csg {


CIE_TEST_CASE( "AABBox", "[partitioning]" )
{
    CIE_TEST_CASE_INIT( "AABBox" )

    {
        CIE_TEST_CASE_INIT( "2D floating point" )

        using BoundingBox = AABBox<2,Double>;

        BoundingBox testBox;
        BoundingBox referenceBox(
            { 0.0, 0.0 },
            { 1.0, 1.0 }
        );

        // Define lengths
        typename BoundingBox::coordinate_type L  = 1.0;
        typename BoundingBox::coordinate_type L2 = 2.0 * L;
        typename BoundingBox::coordinate_type hL = L / 2.0;
        typename BoundingBox::coordinate_type l  = 0.1;
        typename BoundingBox::coordinate_type hl = l / 2.0;

        /* --- y: negative side --- */

        // x: negative side
        // -> completely outside
        testBox = BoundingBox( { -L2, -L2 },
                               { l, l } );
        CHECK( referenceBox.contains(testBox) == false );
        CHECK( referenceBox.intersects(testBox) == false );

        // x: intersect negative side
        // -> completely outside
        testBox = BoundingBox( { -hl, -L2 },
                               { l, l } );
        CHECK( referenceBox.contains(testBox) == false );
        CHECK( referenceBox.intersects(testBox) == false );

        // x: inside
        // -> completely outside
        testBox = BoundingBox( { hL-hl, -L2 },
                               { l, l } );
        CHECK( referenceBox.contains(testBox) == false );
        CHECK( referenceBox.intersects(testBox) == false );

        // x: intersect positive side
        // -> completely outside
        testBox = BoundingBox( { L-hl, -L2 },
                               { l, l } );
        CHECK( referenceBox.contains(testBox) == false );
        CHECK( referenceBox.intersects(testBox) == false );

        // x: positive side
        // -> completely outside
        testBox = BoundingBox( { L2, -L2 },
                               { l, l } );
        CHECK( referenceBox.contains(testBox) == false );
        CHECK( referenceBox.intersects(testBox) == false );

        /* --- y: intersect negative side --- */

        // x: negative side
        // -> completely outside
        testBox = BoundingBox( { -L2, -hl },
                               { l, l } );
        CHECK( referenceBox.contains(testBox) == false );
        CHECK( referenceBox.intersects(testBox) == false );

        // x: intersect negative side
        // -> completely outside
        testBox = BoundingBox( { -hl, -hl },
                               { l, l } );
        CHECK( referenceBox.contains(testBox) == false );
        CHECK( referenceBox.intersects(testBox) == true );

        // x: inside
        // -> completely outside
        testBox = BoundingBox( { hL-hl, -hl },
                               { l, l } );
        CHECK( referenceBox.contains(testBox) == false );
        CHECK( referenceBox.intersects(testBox) == true );

        // x: intersect positive side
        // -> completely outside
        testBox = BoundingBox( { L-hl, -hl },
                               { l, l } );
        CHECK( referenceBox.contains(testBox) == false );
        CHECK( referenceBox.intersects(testBox) == true );

        // x: positive side
        // -> completely outside
        testBox = BoundingBox( { L2, -hl },
                               { l, l } );
        CHECK( referenceBox.contains(testBox) == false );
        CHECK( referenceBox.intersects(testBox) == false );

        /* --- y: inside --- */

        // x: negative side
        // -> completely outside
        testBox = BoundingBox( { -L2, hL-hl },
                               { l, l } );
        CHECK( referenceBox.contains(testBox) == false );
        CHECK( referenceBox.intersects(testBox) == false );

        // x: intersect negative side
        // -> completely outside
        testBox = BoundingBox( { -hl, hL-hl },
                               { l, l } );
        CHECK( referenceBox.contains(testBox) == false );
        CHECK( referenceBox.intersects(testBox) == true );

        // x: inside
        // -> completely outside
        testBox = BoundingBox( { hL-hl, hL-hl },
                               { l, l } );
        CHECK( referenceBox.contains(testBox) == true );
        CHECK( referenceBox.intersects(testBox) == false );

        // x: intersect positive side
        // -> completely outside
        testBox = BoundingBox( { L-hl, hL-hl },
                               { l, l } );
        CHECK( referenceBox.contains(testBox) == false );
        CHECK( referenceBox.intersects(testBox) == true );

        // x: positive side
        // -> completely outside
        testBox = BoundingBox( { L2, hL-hl },
                               { l, l } );
        CHECK( referenceBox.contains(testBox) == false );
        CHECK( referenceBox.intersects(testBox) == false );

        /* --- y: intersect positive side --- */

        // x: negative side
        // -> completely outside
        testBox = BoundingBox( { -L2, L-hl },
                               { l, l } );
        CHECK( referenceBox.contains(testBox) == false );
        CHECK( referenceBox.intersects(testBox) == false );

        // x: intersect negative side
        // -> completely outside
        testBox = BoundingBox( { -hl, L-hl },
                               { l, l } );
        CHECK( referenceBox.contains(testBox) == false );
        CHECK( referenceBox.intersects(testBox) == true );

        // x: inside
        // -> completely outside
        testBox = BoundingBox( { hL-hl, L-hl },
                               { l, l } );
        CHECK( referenceBox.contains(testBox) == false );
        CHECK( referenceBox.intersects(testBox) == true );

        // x: intersect positive side
        // -> completely outside
        testBox = BoundingBox( { L-hl, L-hl },
                               { l, l } );
        CHECK( referenceBox.contains(testBox) == false );
        CHECK( referenceBox.intersects(testBox) == true );

        // x: positive side
        // -> completely outside
        testBox = BoundingBox( { L2, L-hl },
                               { l, l } );
        CHECK( referenceBox.contains(testBox) == false );
        CHECK( referenceBox.intersects(testBox) == false );

        /* --- y: positive side --- */

        // x: negative side
        // -> completely outside
        testBox = BoundingBox( { -L2, L2 },
                               { l, l } );
        CHECK( referenceBox.contains(testBox) == false );
        CHECK( referenceBox.intersects(testBox) == false );

        // x: intersect negative side
        // -> completely outside
        testBox = BoundingBox( { -hl, L2 },
                               { l, l } );
        CHECK( referenceBox.contains(testBox) == false );
        CHECK( referenceBox.intersects(testBox) == false );

        // x: inside
        // -> completely outside
        testBox = BoundingBox( { hL-hl, L2 },
                               { l, l } );
        CHECK( referenceBox.contains(testBox) == false );
        CHECK( referenceBox.intersects(testBox) == false );

        // x: intersect positive side
        // -> completely outside
        testBox = BoundingBox( { L-hl, L2 },
                               { l, l } );
        CHECK( referenceBox.contains(testBox) == false );
        CHECK( referenceBox.intersects(testBox) == false );

        // x: positive side
        // -> completely outside
        testBox = BoundingBox( { L2, L2 },
                               { l, l } );
        CHECK( referenceBox.contains(testBox) == false );
        CHECK( referenceBox.intersects(testBox) == false );

        /* --- Marginal cases --- */

        // Touch negative side from the inside
        testBox = BoundingBox( { 0.0, hL-hl },
                               { l, l } );
        CHECK( referenceBox.contains(testBox) == true );
        CHECK( referenceBox.intersects(testBox) == false );

        // Touch positive side from the outside
        testBox = BoundingBox( { L, hL-hl },
                               { l, l } );
        CHECK( referenceBox.contains(testBox) == false );
        CHECK( referenceBox.intersects(testBox) == false );

        // Touch negative corner from the inside
        testBox = BoundingBox( { 0.0, 0.0 },
                               { l, l } );
        CHECK( referenceBox.contains(testBox) == true );
        CHECK( referenceBox.intersects(testBox) == false );

        // Touch positive corner from the outside
        testBox = BoundingBox( { L, L },
                               { l, l } );
        CHECK( referenceBox.contains(testBox) == false );
        CHECK( referenceBox.intersects(testBox) == false );

        /* --- Box expansion --- */

        testBox = BoundingBox( { 0.0, 0.0 },
                               { L2, l } );
        CHECK_NOTHROW( referenceBox.include(testBox) );
        CHECK( referenceBox.base()[0] == Approx(0.0) );
        CHECK( referenceBox.base()[1] == Approx(0.0) );
        CHECK( referenceBox.lengths()[0] == Approx(L2) );
        CHECK( referenceBox.lengths()[1] == Approx(L) );

        testBox = BoundingBox( { -l, -hl },
                               { l, L2 } );
        CHECK_NOTHROW( referenceBox.include(testBox) );
        CHECK( referenceBox.base()[0] == Approx(-l) );
        CHECK( referenceBox.base()[1] == Approx(-hl) );
        CHECK( referenceBox.lengths()[0] == Approx(L2 + l) );
        CHECK( referenceBox.lengths()[1] == Approx(L2) );
    }
}


} // namespace cie::csg