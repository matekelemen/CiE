#ifndef CIE_GL_SCENE_HPP
#define CIE_GL_SCENE_HPP

// --- External Includes ---
#include <glad/glad.h>
#include <glm/glm.hpp>

// --- Utility Includes ---
#include "cieutils/packages/logging/inc/Loggee.hpp"
#include "cieutils/packages/types/inc/IDObject.hpp"

// --- Internal Includes ---
#include "ciegl/packages/camera/inc/AbsCamera.hpp"
#include "ciegl/packages/shaders/inc/Shader.hpp"
#include "ciegl/packages/buffer/inc/AbsBufferManager.hpp"
#include "ciegl/packages/scene/inc/GLUniform.hpp"

// --- STL Includes ---
#include <memory>
#include <list>


namespace cie::gl {


class Scene :
    public utils::Loggee,
    public utils::IDObject<Size>
{
public:
    using camera_container  = std::list<CameraPtr>;
    using uniform_container = std::list<GLUniformPtr>;

public:

    /**
     * Create new GL program, collect shaders, link program,
     * create a new vertex array object, and activate the program.
     * 
     * NOTE: the scene must be constructed with no bound buffers,
     *       but will bind them during construction. New buffers are
     *       generated if none are supplied.
     */
    Scene( utils::Logger& r_logger,
           const std::string& r_name,
           ShaderPtr p_vertexShader,
           ShaderPtr p_geometryShader,
           ShaderPtr p_fragmentShader,
           BufferManagerPtr p_bufferManager,
           VertexBufferPtr p_vertexBuffer   = nullptr,
           ElementBufferPtr p_elementBuffer = nullptr );

    ~Scene();

    /**
     * Update all cameras and buffers.
     */
    virtual void update();

    /**
     * Create a new camera and add it to this Scene
     */
    template <class CameraType, class ...Args>
    CameraPtr makeCamera( Args&&... args );

    /**
     * Add a camera to this Scene
     */
    void addCamera( CameraPtr p_camera );

    /**
     * Remove camera from this Scene
     */
    void removeCamera( CameraPtr p_camera );

    const camera_container& cameras() const;

    virtual Scene& setVertexShader( ShaderPtr p_vertexShader );
    virtual Scene& setGeometryShader( ShaderPtr p_geometryShader );
    virtual Scene& setFragmentShader( ShaderPtr p_fragmentShader );

    const ShaderPtr& vertexShader() const;
    const ShaderPtr& geometryShader() const;
    const ShaderPtr& fragmentShader() const;

    Scene& setBufferManager( BufferManagerPtr p_bufferManager );
    virtual const BufferManagerPtr& bufferManager() const;

    const uniform_container& uniforms() const;

    /**
     * Bind a 4x4 float matrix to a uniform with the specified name
     */
    void bindUniform( const std::string& r_name,
                      const glm::mat4& r_uniform );

private:
    /**
     * Find uniform in the internal list by its name
     */
    GLUniformPtr& findUniform( const std::string& r_name );

protected:
    camera_container    _cameras;

    ShaderPtr           _p_vertexShader;
    ShaderPtr           _p_geometryShader;
    ShaderPtr           _p_fragmentShader;

    BufferManagerPtr    _p_bufferManager;

    uniform_container   _uniforms;

    GLuint              _vaoID;
};


using ScenePtr  = std::shared_ptr<Scene>;


} // namespace cie::gl

#include "ciegl/packages/scene/impl/Scene_impl.hpp"

#endif