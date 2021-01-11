#ifndef CIE_GL_SCENE_AXES_3D_SCENE_HPP
#define CIE_GL_SCENE_AXES_3D_SCENE_HPP

// --- Internal Includes ---
#include "ciegl/packages/scene/inc/GenericPartScene.hpp"
#include "ciegl/packages/buffer/inc/ColoredVertex3.hpp"
#include "ciegl/packages/shapes/inc/Arrow.hpp"
#include "ciegl/packages/traits/inc/GLTraits.hpp"

// --- STL Includes ---
#include <vector>


namespace cie::gl {


class Axes3DScene :
    public GenericPartScene,
    public GLTraits
{
public:
    using arrow_type      = Arrow<ColoredVertex3>;
    using arrow_container = std::vector<arrow_type>;

public:
    Axes3DScene( utils::Logger& r_logger,
                 const std::string& r_name,
                 CameraPtr p_camera );

protected:
    arrow_container _arrows;
};





} // namespace cie::gl


#endif