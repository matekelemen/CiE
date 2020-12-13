// --- Utility Includes ---
#include "cieutils/packages/testing/inc/essentials.hpp"
#include "cieutils/packages/logging/inc/Logger.hpp"
#include "cmake_variables.hpp"

// --- Internal Includes ---
#include "ciegl/packages/camera/inc/Camera.hpp"
#include "ciegl/packages/camera/inc/OrthographicProjection.hpp"
#include "ciegl/packages/camera/inc/PerspectiveProjection.hpp"

// --- STL Includes ---
#include <iostream>


namespace cie::gl {


CIE_TEST_CASE( "Camera", "[camera]" )
{
    CIE_CIE_TEST_CASE_INIT( "Camera" )

    utils::Logger logger( TEST_OUTPUT_PATH / "Camera_test.txt" );

    {
        CIE_CIE_TEST_CASE_INIT( "OrthographicProjection" )

        using CameraType = Camera<OrthographicProjection>;
        CameraPtr p_camera;

        REQUIRE_NOTHROW( p_camera = CameraPtr(
            new CameraType( logger )
        ));
    }

    {
        CIE_CIE_TEST_CASE_INIT( "PerspectiveProjection" )

        using CameraType = Camera<PerspectiveProjection>;
        CameraPtr p_camera;

        REQUIRE_NOTHROW( p_camera = CameraPtr(
            new CameraType( logger )
        ));
    }
}


} // namespace cie::gl