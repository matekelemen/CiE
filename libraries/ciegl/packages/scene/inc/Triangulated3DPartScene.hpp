#ifndef CIE_GL_TRIANGULATED_3D_PART_SCENE_HPP
#define CIE_GL_TRIANGULATED_3D_PART_SCENE_HPP

// --- Internal Includes ---
#include "ciegl/packages/scene/inc/PartScene.hpp"


namespace cie::gl {


class Triangulated3DPartScene : public PartScene
{
public:
    // Constructor for the AbsWindow::makeScene
    Triangulated3DPartScene( utils::Logger& r_logger,
                             const std::string& r_name,
                             Triangulated3DPartScene::part_container&& r_parts,
                             CameraPtr p_camera = nullptr );

    Triangulated3DPartScene( utils::Logger& r_logger,
                             Triangulated3DPartScene::part_container&& r_parts,
                             CameraPtr p_camera = nullptr );

protected:
    virtual void update_impl() override;

    /// Constructor for derived classes
    Triangulated3DPartScene( utils::Logger& r_logger,
                             const std::string& r_name,
                             part_container&& r_parts,
                             CameraPtr p_camera,
                             ShaderPtr p_vertexShader,
                             ShaderPtr p_geometryShader,
                             ShaderPtr p_fragmentShader,
                             VertexBufferPtr p_vertexBuffer,
                             ElementBufferPtr p_elementBuffer );
};


} // namespace cie::gl


#endif