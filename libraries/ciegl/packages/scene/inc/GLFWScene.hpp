#ifndef CIE_GL_GLFW_SCENE_HPP
#define CIE_GL_GLFW_SCENE_HPP

// --- External Includes ---
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// --- Utility Includes ---
#include "cieutils/packages/types/inc/IDObject.hpp"

// --- Internal Includes ---
#include "ciegl/packages/scene/inc/Scene.hpp"


namespace cie::gl {


class GLFWScene :
    public Scene,
    public utils::IDObject<Size>
{
public:

    /**
     * Create new GL program, collect shaders, link program,
     * create a new vertex array object, and activate the program.
     */
    GLFWScene( utils::Logger& r_logger,
               ShaderPtr p_vertexShader,
               ShaderPtr p_geometryShader,
               ShaderPtr p_fragmentShader );

    ~GLFWScene();

protected:
    GLuint _vaoID;
};


} // namespace cie::gl


#endif