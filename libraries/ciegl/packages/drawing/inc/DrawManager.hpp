#ifndef CIEGL_DRAW_MANAGER_HPP
#define CIEGL_DRAW_MANAGER_HPP

// --- Internal Includes ---
#include "ProgramManager.hpp"
#include "ciegl/packages/shaders/inc/ShaderManager.hpp"
#include "ciegl/packages/drawing/inc/Camera.hpp"

namespace cie::gl {


class DrawManager : public ProgramManager
{
public:
    DrawManager(    GLContext& context,
                    const std::string& className = "DrawManager" );
    ~DrawManager();

    void initialize() override;
    void compileShaders() override;
    void makeProgram() override;

    virtual bool draw();
    DrawFunction makeDrawFunction( GLContext& context );

    ShaderManager& shaderManager();
    const ShaderManager& shaderManager() const;
    CameraPtr& camera();
    const CameraPtr& camera() const;

protected:
    ShaderManager   _shaderManager;
    CameraPtr       _camera;

private:
    std::vector<GLint>  _uniformIDs;
    GLint               _transformationID;
};


} // namespace cie::gl

#endif