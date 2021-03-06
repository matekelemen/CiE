#ifndef CIE_GL_SCENE_GENERIC_PART_SCENE_HPP
#define CIE_GL_SCENE_GENERIC_PART_SCENE_HPP

// --- Internal Includes ---
#include "ciegl/packages/scene/inc/PartScene.hpp"

// --- STL Includes ---
#include <filesystem>


namespace cie::gl {


/// Convenience Scene class
class GenericPartScene : public PartScene
{
public:
    GenericPartScene( const std::string& r_name,
                      Size drawMode,
                      GenericPartScene::part_container&& r_parts,
                      ShaderPtr p_vertexShader,
                      ShaderPtr p_geometryShader,
                      ShaderPtr p_fragmentShader,
                      utils::Logger& r_logger,
                      VertexBufferPtr p_vertexBuffer,
                      ElementBufferPtr p_elementBuffer );

    GenericPartScene( const std::string& r_name,
                      Size drawMode,
                      const std::filesystem::path& r_shaderDirectory,
                      utils::Logger& r_logger = utils::LoggerSingleton::get() );

    void setDrawMode( Size drawMode );

protected:
    virtual void update_impl() override;

protected:
    Size _drawMode;
};


} // namespace cie::gl


#endif