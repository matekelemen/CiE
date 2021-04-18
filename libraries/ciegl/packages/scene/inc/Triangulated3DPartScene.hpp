#ifndef CIE_GL_TRIANGULATED_3D_PART_SCENE_HPP
#define CIE_GL_TRIANGULATED_3D_PART_SCENE_HPP

// --- Internal Includes ---
#include "ciegl/packages/scene/inc/PartScene.hpp"


namespace cie::gl {


class Triangulated3DPartScene : public PartScene
{
public:
    // Constructor for the AbsWindow::makeScene
    Triangulated3DPartScene( const std::string& r_name,
                             Triangulated3DPartScene::part_container&& r_parts,
                             utils::Logger& r_logger = utils::LoggerSingleton::get() );

protected:
    virtual void update_impl() override;

    /// Constructor for derived classes
    Triangulated3DPartScene( const std::string& r_name,
                             part_container&& r_parts,
                             CameraPtr p_camera,
                             ShaderPtr p_vertexShader,
                             ShaderPtr p_geometryShader,
                             ShaderPtr p_fragmentShader,
                             utils::Logger& r_logger,
                             VertexBufferPtr p_vertexBuffer,
                             ElementBufferPtr p_elementBuffer );
};


} // namespace cie::gl


#endif