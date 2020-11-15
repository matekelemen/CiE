// --- Internal Includes ---
#include "ciegl/packages/control/inc/MappedCameraControls.hpp"


namespace cie::gl {


MappedCameraControls::MappedCameraControls( WindowPtr p_window,
                                            CameraPtr p_camera ) :
    AbsCameraControls( p_window, p_camera ),
    MappedControls()
{
}


void MappedCameraControls::onKeyboardPress( KeyEnum key,
                                            KeyEnum modifiers )
{
    this->callControl( key );
}


} // namespace cie::gl