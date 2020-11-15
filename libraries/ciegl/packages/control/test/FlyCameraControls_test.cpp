// --- External Includes ---
#include "catch.hpp"

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/testing.hpp"

// --- Internal Includes ---
#include "ciegl/packages/control/inc/FlyCameraControls.hpp"
#include "ciegl/packages/camera/inc/AbsCamera.hpp"
#include "ciegl/packages/context/inc/AbsWindow.hpp"


namespace cie::gl {


TEST_CASE( "FlyCameraControls", "[control]" )
{
    CIE_TEST_CASE_INIT( "FlyCameraControls" )

//    WindowPtr p_window;
//    CameraPtr p_camera;
//
//    CameraControlsPtr p_controls;
//    
//    REQUIRE_NOTHROW( p_controls.reset(
//        new FlyCameraControls( p_window, p_camera )
//    ));
}


} // namespace cie::gl