// --- Internal Includes ---
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
    _farClippingPlane( 10.0f ),
    _viewMatrix( 1.0f ),
    _projectionMatrix( 1.0f ),
    _transformationMatrix( 1.0f )
{
    update();
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
    update();
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
    update();

    return *this;
}


Camera::~Camera()
{
}


void Camera::zoom( GLfloat modifier )
{
    if ( modifier>0 )
    {
        _height /= (1.0 + modifier);
        _width  /= (1.0 + modifier);
    }
    else
    {
        _height *= (modifier - 1.0);
        _width  *= (modifier - 1.0);
    }
    

    if ( _fieldOfView!=0.0 &&_fieldOfView > 10*modifier )
        setProperties(  _fieldOfView - 10*modifier,
                        _nearClippingPlane,
                        _farClippingPlane   );

    updateTransformationMatrix();
}


void Camera::translate( const glm::vec3& translation )
{
    setPose(    _cameraPosition + translation,
                _cameraDirection );
    updateTransformationMatrix();
}


void Camera::rotate(    GLfloat degrees,
                        const glm::vec3& axis )
{
    auto rotationMatrix = glm::rotate(  glm::radians(degrees),
                                        axis );
    auto pos    = rotationMatrix * glm::vec4(_cameraPosition, 1.0f);
    auto dir    = rotationMatrix * glm::vec4(_cameraDirection, 1.0f);

    setPose( glm::vec3(pos), glm::vec3(dir) );
    updateTransformationMatrix();
}


void Camera::setProperties( GLfloat fieldOfView,
                            GLfloat nearClippingPlane,
                            GLfloat farClippingPlane )
{
    _fieldOfView        = fieldOfView;
    _nearClippingPlane  = nearClippingPlane;
    _farClippingPlane   = farClippingPlane;

    int w, h;
    if (_context->window() != nullptr)
        glfwGetFramebufferSize( _context->window(), &w, &h );
    else
    {
        auto windowSizes    = getPrimaryMonitorResolution();
        w                   = windowSizes.first;
        h                   = windowSizes.second;
    }

    if (abs(_fieldOfView) > 1e-5)
        _projectionMatrix = glm::perspective(   glm::radians(_fieldOfView),
                                                (GLfloat)w / (GLfloat)h,
                                                _nearClippingPlane,
                                                _farClippingPlane );
    else
        _projectionMatrix = glm::ortho( -w/2.0f,
                                        w/2.0f,
                                        -h/2.0f,
                                        h/2.0f,
                                        _nearClippingPlane,
                                        _farClippingPlane   );
}


void Camera::setProperties( GLfloat fieldOfView )
{
    setProperties(  fieldOfView,
                    _nearClippingPlane,
                    _farClippingPlane   );
}


void Camera::updateTransformationMatrix()
{
    _transformationMatrix = _projectionMatrix * _viewMatrix;
}


void Camera::update()
{
    setPose(    _cameraPosition,
                _cameraDirection );
    setProperties(  _fieldOfView,
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



InteractiveCamera::InteractiveCamera( GLContext& context ) :
    Camera( context ),
    _mousePressPosition( {0.0,0.0,0.0} )
{
}


InteractiveCamera::InteractiveCamera( const InteractiveCamera& copy ):
    Camera( copy ),
    _mousePressPosition( copy._mousePressPosition )
{
}


glm::vec3 InteractiveCamera::screenToWorld( double x, double y ) const
{
    // Get window size
    int w, h;
    glfwGetFramebufferSize( _context->window(), &w, &h );

    // Map to [-1,1] (with civilized y-axis)
    x   = 2.0 * x/(double)w - 1.0;
    y   = -(2.0 * y/(double)h - 1.0);

    // Transform to world
    glm::vec4 world = glm::inverse(_transformationMatrix) * glm::vec4( x, y, 1.0f, 1.0f );
    
    return glm::vec3(world);
}


void InteractiveCamera::setMousePressPosition( double x, double y )
{
    _mousePressPosition = screenToWorld( x, y );
}


void InteractiveCamera::setMousePressPosition( const glm::vec3& mousePos )
{
    _mousePressPosition = mousePos;
}


const glm::vec3& InteractiveCamera::mousePressPosition( ) const
{
    return _mousePressPosition;
}


}
}