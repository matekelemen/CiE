#ifndef CIE_GL_MAPPED_CAMERA_CONTROLS_HPP
#define CIE_GL_MAPPED_CAMERA_CONTROLS_HPP

// --- Internal Includes ---
#include "ciegl/packages/control/inc/MappedControls.hpp"
#include "ciegl/packages/control/inc/AbsCameraControls.hpp"


namespace cie::gl {


class MappedCameraControls :
    public AbsCameraControls,
    public MappedControls
{
public:
    MappedCameraControls( WindowPtr p_window,
                          CameraPtr p_camera );

protected:
    virtual void onKeyboardPress( KeyEnum key,
                                  KeyEnum modifiers ) override;
}; 


} // namespace cie::gl


#endif