#ifndef CIE_GL_ABS_CAMERA_CONTROLS_HPP
#define CIE_GL_ABS_CAMERA_CONTROLS_HPP

// --- Internal Includes ---
#include "ciegl/packages/context/inc/AbsWindow.hpp"
#include "ciegl/packages/camera/inc/AbsCamera.hpp"
#include "ciegl/packages/control/inc/callback_types.hpp"

// --- STL Includes ---
#include <functional>
#include <memory>


namespace cie::gl {


class AbsCameraControls
{
public:
    AbsCameraControls( WindowPtr p_window,
                       CameraPtr p_camera );

    void mouseButtonCallback( KeyEnum button,
                              KeyEnum action,
                              KeyEnum modifiers );

    void cursorPositionCallback( double x,
                                 double y );

    void cursorEnterCallback( KeyEnum entered );

    void scrollCallback( double xOffset,
                         double yOffset );

    void keyboardCallback( KeyEnum key,
                           KeyEnum action,
                           KeyEnum modifiers );

    void bindToWindow( WindowPtr p_window );

protected:
    virtual void onMouseButtonPress( KeyEnum button,
                                     KeyEnum modifiers )
    {}
                                    
    virtual void onMouseButtonRelease( KeyEnum button,
                                       KeyEnum modifiers )
    {}

    virtual void onCursorMovement( double x,
                                   double y )
    {}

    virtual void onCursorEnter()
    {}

    virtual void onCursorLeave()
    {}

    virtual void onHorizontalScroll( double offset )
    {}

    virtual void onVerticalScroll( double offset )
    {}

    virtual void onKeyboardPress( KeyEnum key,
                                  KeyEnum modifiers );

    virtual void onKeyboardRelease( KeyEnum key,
                                    KeyEnum modifiers )
    {}

protected:
    WindowPtr _p_window;
    CameraPtr _p_camera;
    double    _x;
    double    _y;
};


using CameraControlsPtr = std::shared_ptr<AbsCameraControls>;


} // namespace cie::gl


#endif