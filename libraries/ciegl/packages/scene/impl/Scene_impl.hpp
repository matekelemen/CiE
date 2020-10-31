#ifndef CIE_GL_SCENE_IMPL_HPP
#define CIE_GL_SCENE_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"


namespace cie::gl {


template <class CameraType, class ...Args>
inline CameraPtr
Scene::makeCamera( Args&&... args )
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto p_camera = CameraPtr(
        new CameraType( this->logger(), std::forward<Args>(args)... )
    );

    this->addCamera( p_camera );

    return p_camera;

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::gl


#endif