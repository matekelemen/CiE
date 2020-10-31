// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- Internal Includes ---
#include "ciegl/packages/scene/inc/Scene.hpp"

// --- STL Includes ---
#include <algorithm>


namespace cie::gl {


Scene::Scene( utils::Logger& r_logger,
              const std::string& r_name ) :
    utils::Loggee( r_logger, r_name ),
    _cameras(),
    _p_vertexShader( nullptr ),
    _p_geometryShader( nullptr ),
    _p_fragmentShader( nullptr )
{
}


void Scene::update()
{
    CIE_BEGIN_EXCEPTION_TRACING

    this->update_impl();

    for ( auto& rp_camera : _cameras )
        rp_camera->update();

    CIE_END_EXCEPTION_TRACING
}


void Scene::addCamera( CameraPtr p_camera )
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto it_camera = std::find(
        _cameras.begin(),
        _cameras.end(),
        p_camera
    );

    if ( it_camera != _cameras.end() )
        CIE_THROW( Exception, "Camera is already part of this Scene!" )

    _cameras.push_back( p_camera );

    CIE_END_EXCEPTION_TRACING
}


void Scene::removeCamera( CameraPtr p_camera )
{
    CIE_BEGIN_EXCEPTION_TRACING

    auto it_camera = std::find(
        _cameras.begin(),
        _cameras.end(),
        p_camera
    );

    if ( it_camera == _cameras.end() )
        CIE_THROW( OutOfRangeException, "Attempt to remove non-existent camera" )

    _cameras.erase( it_camera );

    CIE_END_EXCEPTION_TRACING
}


const typename Scene::camera_container& Scene::cameras() const
{
    return _cameras;
}


Scene& Scene::setVertexShader( ShaderPtr p_vertexShader )
{
    _p_vertexShader = p_vertexShader;
    return *this;
}


Scene& Scene::setGeometryShader( ShaderPtr p_geometryShader )
{
    _p_geometryShader = p_geometryShader;
    return *this;
}


Scene& Scene::setFragmentShader( ShaderPtr p_fragmentShader )
{
    _p_fragmentShader = p_fragmentShader;
    return *this;
}


const ShaderPtr& Scene::vertexShader() const
{
    return _p_vertexShader;
}


const ShaderPtr& Scene::geometryShader() const
{
    return _p_geometryShader;
}


const ShaderPtr& Scene::fragmentShader() const
{
    return _p_fragmentShader;
}


Scene& Scene::setBufferManager( BufferManagerPtr p_bufferManager )
{
    _p_bufferManager = p_bufferManager;
    return *this;
}


const BufferManagerPtr& Scene::bufferManager() const
{
    return _p_bufferManager;
}


} // namespace cie::gl