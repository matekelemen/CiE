#include "../inc/Camera.hpp"

namespace cie {
namespace gl {


Camera::Camera( GLContext& context ) :
    AbsContextClass( context, "Camera" ),
    _cameraPosition( {0.0f, 0.0f, 1.0f} ),
    _cameraDirection( {0.0f, 0.0f, -1.0f} ),
    _fieldOfView( 0.0f ),
    _nearClippingPlane( 0.1f ),
    _farClippingPlane( 100.0f ),
    _viewMatrix( 1.0f ),
    _transformationMatrix( 1.0f )
{
    setCameraPose<glm::vec3>(   glm::vec3( 0.0, 0.0, 10.0),
                                glm::vec3( 0.0f, 0.0f, -1.0f) );

    setCameraProperties(    _fieldOfView, 
                            _nearClippingPlane, 
                            _farClippingPlane );

    updateTransformationMatrix();
    log( "Create Camera" );
}


void Camera::setCameraProperties(   float fieldOfView,
                                    float nearClippingPlane,
                                    float farClippingPlane )
{
    _fieldOfView        = fieldOfView;
    _nearClippingPlane  = nearClippingPlane;
    _farClippingPlane   = farClippingPlane;

    int width, height;
    if (_context->window() != nullptr)
        glfwGetWindowSize( _context->window(), &width, &height );
    else
    {
        auto windowSizes    = getPrimaryMonitorResolution();
        width               = windowSizes.first;
        height              = windowSizes.second;
    }

    if (abs(_fieldOfView) > 1e-5)
        _projectionMatrix = glm::perspective(   glm::radians(_fieldOfView),
                                                (GLfloat)width / (GLfloat)height,
                                                _nearClippingPlane,
                                                _farClippingPlane );
    else
        _projectionMatrix = glm::ortho( -1.0f,
                                        1.0f,
                                        -1.0f,
                                        1.0f,
                                        _nearClippingPlane,
                                        _farClippingPlane   );
}


void Camera::updateTransformationMatrix()
{
    _transformationMatrix = _projectionMatrix * _viewMatrix;
}


const glm::mat4& Camera::viewMatrix() const
{
    return _viewMatrix;
}


const glm::mat4& Camera::projectionMatrix() const
{
    return _projectionMatrix;
}


const glm::mat4& Camera::transformationMatrix() const
{
    return _transformationMatrix;
}




}
}