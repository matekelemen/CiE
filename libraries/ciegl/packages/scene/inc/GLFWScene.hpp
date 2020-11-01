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
     * 
     * NOTE: the scene must be constructed with no bound buffers,
     *       but will bind them during construction. New buffers are
     *       generated if none are supplied.
     */
    GLFWScene( utils::Logger& r_logger,
               const std::string& r_name,
               ShaderPtr p_vertexShader,
               ShaderPtr p_geometryShader,
               ShaderPtr p_fragmentShader,
               BufferManagerPtr p_bufferManager,
               VertexBufferPtr p_vertexBuffer   = nullptr,
               ElementBufferPtr p_elementBuffer = nullptr );

    ~GLFWScene();

protected:
    GLuint _vaoID;
};


} // namespace cie::gl


#endif