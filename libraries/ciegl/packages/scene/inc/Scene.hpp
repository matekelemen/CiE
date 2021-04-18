#ifndef CIE_GL_SCENE_HPP
#define CIE_GL_SCENE_HPP

// --- External Includes ---
#include <glad/glad.h>
#include <glm/glm.hpp>

// --- Utility Includes ---
#include "cieutils/packages/logging/inc/Loggee.hpp"
#include "cieutils/packages/logging/inc/LoggerSingleton.hpp"
#include "cieutils/packages/types/inc/IDObject.hpp"

// --- Internal Includes ---
#include "ciegl/packages/camera/inc/AbsCamera.hpp"
#include "ciegl/packages/shaders/inc/Shader.hpp"
#include "ciegl/packages/buffer/inc/AbsBufferManager.hpp"
#include "ciegl/packages/scene/inc/GLUniform.hpp"
#include "ciegl/packages/texture/inc/Texture.hpp"

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
    using texture_container = std::list<TexturePtr>;

public:

    /**
     * Create new GL program, collect shaders, link program,
     * create a new vertex array object, and activate the program.
     * 
     * @note the scene must be constructed with no bound buffers,
     *       but will bind them during construction. New buffers are
     *       generated if none are supplied.
     */
    Scene( const std::string& r_name,
           ShaderPtr p_vertexShader,
           ShaderPtr p_geometryShader,
           ShaderPtr p_fragmentShader,
           utils::Logger& r_logger = utils::LoggerSingleton::get(),
           VertexBufferPtr p_vertexBuffer   = nullptr,
           ElementBufferPtr p_elementBuffer = nullptr );

    ~Scene();

    /**
     * Update all cameras and uniforms,
     * then call update_impl for all buffers
     */
    virtual void update();

    /**
     * Set this scene as active
     *  -> calls glUseProgram and binds the vertex array object
     */
    void activate( bool log = true );

    /// Check whether this scene is active
    bool isActive() const;

    /// Create a new camera and add it to this Scene
    template <class CameraType, class ...Args>
    CameraPtr makeCamera( Args&&... args );

    /// Add a camera to this Scene
    void addCamera( CameraPtr p_camera );

    /// Remove camera from this Scene
    void removeCamera( CameraPtr p_camera );

    const camera_container& cameras() const;

    CameraPtr camera( Size index = 0 );

    virtual Scene& setVertexShader( ShaderPtr p_vertexShader );
    virtual Scene& setGeometryShader( ShaderPtr p_geometryShader );
    virtual Scene& setFragmentShader( ShaderPtr p_fragmentShader );

    const ShaderPtr& vertexShader() const;
    const ShaderPtr& geometryShader() const;
    const ShaderPtr& fragmentShader() const;

    Scene& setBufferManager( BufferManagerPtr p_bufferManager );
    virtual const BufferManagerPtr& bufferManager() const;

    const uniform_container& uniforms() const;
    const texture_container& textures() const;

    /// Bind a 4x4 float matrix to a uniform with the specified name
    void bindUniform( const std::string& r_name,
                      const glm::mat4& r_uniform );

    /// Bind a 3D double vector to a uniform with the specified name
    void bindUniform( const std::string& r_name,
                      const glm::dvec3& r_uniform );

    /// Bind a 3D float vector to a uniform with the specified name
    void bindUniform( const std::string& r_name,
                      const glm::vec3& r_uniform );

    /// Bind a float scalar to a uniform with the specified name
    void bindUniform( const std::string& r_name,
                      const GLfloat& r_uniform );

    /// Load data to a texture
    template <class ...Args>
    void loadTexture( const std::string& r_name,
                      Args&&... args );

protected:
    /// Implementation-specific drawing
    virtual void update_impl() = 0;

private:
    /// Find uniform in the internal list by its name
    GLUniformPtr& findUniform( const std::string& r_name );

    /// Find texture in the internal list by its name
    TexturePtr& findTexture( const std::string& r_name );

protected:
    camera_container    _cameras;

    ShaderPtr           _p_vertexShader;
    ShaderPtr           _p_geometryShader;
    ShaderPtr           _p_fragmentShader;

    BufferManagerPtr    _p_bufferManager;
    uniform_container   _uniforms;
    texture_container   _textures;
    GLuint              _vaoID;

    static Size         _activeSceneID;
};


using ScenePtr  = std::shared_ptr<Scene>;


} // namespace cie::gl

#include "ciegl/packages/scene/impl/Scene_impl.hpp"

#endif