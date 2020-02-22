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
    DrawManager( GLContext& context );

    void initialize() override;
    void compileShaders() override;
    void makeProgram() override;

    virtual void draw();
    DrawFunction makeDrawFunction( GLContext& context );

    ShaderManager& shaderManager();
    const ShaderManager& shaderManager() const;
    Camera& camera();
    const Camera& camera() const;

protected:
    ShaderManager   _shaderManager;
    Camera          _camera;

private:
    std::vector<GLint> _uniformIDs;
};


}
}

#endif