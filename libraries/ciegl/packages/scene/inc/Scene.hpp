#ifndef CIE_GL_SCENE_HPP
#define CIE_GL_SCENE_HPP

// --- Utility Includes ---
#include "cieutils/packages/logging/inc/Loggee.hpp"

// --- Internal Includes ---
#include "ciegl/packages/camera/inc/AbsCamera.hpp"
#include "ciegl/packages/shaders/inc/Shader.hpp"
#include "ciegl/packages/buffer/inc/AbsBufferManager.hpp"

// --- STL Includes ---
#include <memory>
#include <list>


namespace cie::gl {


class Scene : public utils::Loggee
{
public:
    using camera_container  = std::list<CameraPtr>;

public:

    Scene( utils::Logger& r_logger,
           const std::string& r_name = "Scene" );

    /**
     * Update all cameras and buffers.
     * A pure virtual update_impl is called at the beginning
     * that is responsible for updating objects related to the
     * scene (adjust camera position, generate/move vertices, etc.)
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

protected:
    /**
     * Update buffers and objects related to the scene.
     */
    virtual void update_impl() = 0;

protected:
    camera_container    _cameras;
    ShaderPtr           _p_vertexShader;
    ShaderPtr           _p_geometryShader;
    ShaderPtr           _p_fragmentShader;
    BufferManagerPtr    _p_bufferManager;
};


using ScenePtr  = std::shared_ptr<Scene>;


} // namespace cie::gl

#include "ciegl/packages/scene/impl/Scene_impl.hpp"

#endif