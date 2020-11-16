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
    MappedCameraControls();

protected:
    virtual void onSubjectChange() override;
}; 


} // namespace cie::gl


#endif