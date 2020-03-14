#include "../inc/Camera.hpp"

namespace cie {
namespace gl {


Camera::Camera( GLContext& context ) :
    AbsContextClass( context, "Camera" ),
    _cameraPosition( 0.0f, 0.0f, 1.0f ),
    _cameraDirection( 0.0f, 0.0f, -1.0f ),
    _fieldOfView( 90.0f ),
    _width( 2.0f ),
    _height( 2.0f ),
    _nearClippingPlane( 0.1f ),
    _farClippingPlane( 100.0f ),
    _viewMatrix( 1.0f ),
    _projectionMatrix( 1.0f ),
    _transformationMatrix( 1.0f )
{
    setCameraPose<glm::vec3>(   _cameraPosition,
                                _cameraDirection );

    setCameraProperties(    _fieldOfView, 
                            _nearClippingPlane, 
                            _farClippingPlane );

    updateTransformationMatrix();
}


Camera::Camera( const Camera& camera ) :
    AbsContextClass( *camera._context, "Camera" ),
    _cameraPosition( camera._cameraPosition ),
    _cameraDirection( camera._cameraDirection ),
    _fieldOfView( camera._fieldOfView ),
    _width( camera._width ),
    _height( camera._height ),
    _nearClippingPlane( camera._nearClippingPlane ),
    _farClippingPlane( camera._farClippingPlane ),
    _viewMatrix( camera._viewMatrix ),
    _projectionMatrix( camera._projectionMatrix ),
    _transformationMatrix( camera._transformationMatrix )
{
}


Camera& Camera::operator=( const Camera& camera )
{
    _cameraPosition         = camera._cameraPosition;
    _cameraDirection        = camera._cameraDirection;
    _fieldOfView            = camera._fieldOfView;
    _width                  = camera._width;
    _height                 = camera._height;
    _nearClippingPlane      = camera._nearClippingPlane;
    _farClippingPlane       = camera._farClippingPlane;
    _viewMatrix             = camera._viewMatrix;
    _projectionMatrix       = camera._projectionMatrix;
    _transformationMatrix   = camera._transformationMatrix;

    return *this;
}


Camera::~Camera()
{
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
        glfwGetFramebufferSize( _context->window(), &width, &height );
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
        _projectionMatrix = glm::ortho( -width/2.0f,
                                        width/2.0f,
                                        -height/2.0f,
                                        height/2.0f,
                                        _nearClippingPlane,
                                        _farClippingPlane   );
}


void Camera::updateTransformationMatrix()
{
    _transformationMatrix = _projectionMatrix * _viewMatrix;
}


void Camera::update()
{
    setCameraPose(  _cameraPosition,
                    _cameraDirection );
    setCameraProperties(    _fieldOfView,
                            _nearClippingPlane,
                            _farClippingPlane   );
    updateTransformationMatrix();
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


GLfloat& Camera::width()
{
    return _width;
}


GLfloat& Camera::height()
{
    return _height;
}


const glm::vec3& Camera::cameraPosition() const
{
    return _cameraPosition;
}


const glm::vec3& Camera::cameraDirection() const
{
    return _cameraDirection;
}


GLfloat Camera::fieldOfView() const
{
    return _fieldOfView;
}


GLfloat Camera::nearClippingPlane() const
{
    return _nearClippingPlane;
}


GLfloat Camera::farClippingPlane() const
{
    return _farClippingPlane;
}



ArcballCamera::ArcballCamera( GLContext& context ) :
    Camera( context ),
    _mousePressPosition( {0.0,0.0} )
{
    std::cout << "ArcballCamera default constructor\n";
}


ArcballCamera::ArcballCamera( const ArcballCamera& camera ):
    Camera( camera ),
    _mousePressPosition( camera._mousePressPosition )
{
    std::cout << "ArcballCamera copy constructor\n";
}


void ArcballCamera::setMousePressPosition( double x, double y )
{
    _mousePressPosition[0] = x;
    _mousePressPosition[1] = y;
}


const std::array<double, 2>& ArcballCamera::mousePressPosition( ) const
{
    return _mousePressPosition;
}


}
}