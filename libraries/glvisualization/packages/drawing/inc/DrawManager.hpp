#ifndef GLVISUALIZATION_DRAW_MANAGER_HPP
#define GLVISUALIZATION_DRAW_MANAGER_HPP

// --- Internal Includes ---
#include "ProgramManager.hpp"
#include "../../shaders/inc/ShaderManager.hpp"
#include "../../drawing/inc/Camera.hpp"

namespace cie {
namespace gl {


class DrawManager : public ProgramManager
{
public:
    DrawManager(    GLContext& context,
                    const std::string& className = "DrawManager" );
    ~DrawManager();

    void initialize() override;
    void compileShaders() override;
    void makeProgram() override;

    virtual void draw();
    DrawFunction makeDrawFunction( GLContext& context );

    void checkGLErrors( const std::string& errorMessage = "" );

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


}
}

#endif