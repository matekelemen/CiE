#ifndef CIE_GL_SCENE_AXES_3D_SCENE_HPP
#define CIE_GL_SCENE_AXES_3D_SCENE_HPP

// --- Internal Includes ---
#include "ciegl/packages/scene/inc/GenericPartScene.hpp"
#include "ciegl/packages/camera/inc/AbsCamera.hpp"
#include "ciegl/packages/shapes/inc/Axes.hpp"
#include "ciegl/packages/traits/inc/GLTraits.hpp"

// --- STL Includes ---
#include <vector>
#include <array>
#include <utility>


namespace cie::gl {


class Axes3DScene :
    public GenericPartScene,
    public GLTraits
{
public:
    using axes_ptr   = std::shared_ptr<Axes>;
    using screen_box = std::array<std::pair<double,double>,2>;

public:
    Axes3DScene( utils::Logger& r_logger,
                 const std::string& r_name,
                 CameraPtr p_camera );

protected:
    virtual void update_impl() override;

protected:
    axes_ptr                        _p_axes;
    screen_box                      _box;
    AbsCamera::internal_matrix_type _transformationMatrix;
};





} // namespace cie::gl


#endif