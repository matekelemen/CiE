#ifndef CIE_GL_PERSPECTIVE_PROJECTION_HPP
#define CIE_GL_PERSPECTIVE_PROJECTION_HPP

// --- Internal Includes ---
#include "ciegl/packages/camera/inc/ProjectionPolicy.hpp"


namespace cie::gl {


class PerspectiveProjection : public ProjectionPolicy
{
protected:
    void zoom_impl( double scale ) override;
    void updateProjectionMatrix_impl() override;
};


} // namespace cie::gl


#endif