// --- Internal Includes ---
#include "../inc/Camera.hpp"

// --- External Includes ---
#include <glm/gtx/rotate_vector.hpp>

// --- STD Includes ---
#include <cmath>

namespace cie {
namespace gl {


Camera::Camera( GLContext& context, const std::string& className ) :
    AbsContextClass( context, className ),
    _cameraPosition( 0.0f, 0.0f, 1.0f ),
    _cameraDirection( 0.0f, 0.0f, -1.0f ),
    _cameraUp( 0.0f, 1.0f, 0.0f ),
    _fieldOfView( 90.0f ),
    _nearClippingPlane( 0.1f ),
    _farClippingPlane( 10.0f ),
    _viewMatrix( 1.0f ),
    _projectionMatrix( 1.0f ),
    _transformationMatrix( 1.0f )
{
    // Update width and height (orthographic mode)
    int w, h;

    if (_context->window() != nullptr)
    {
        glfwGetFramebufferSize( _context->window(), &w, &h );
    }
    else
    {
        auto resolution = getPrimaryMonitorResolution();
        w = (int)resolution.first;
        h = (int)resolution.second;
    }
    
    _width      = 2.0;
    _height     = 2.0 * (double)h / (double)w;
    update();
}


Camera::Camera( const Camera& camera ) :
    AbsContextClass( *camera._context, "Camera" ),
    _cameraPosition( camera._cameraPosition ),
    _cameraDirection( camera._cameraDirection ),
    _cameraUp( camera._cameraUp ),
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
    _cameraUp               = camera._cameraUp;
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
    _height /= (1.0 + modifier/10.0);
    _width  /= (1.0 + modifier/10.0);
    

    if ( _fieldOfView!=0.0 &&_fieldOfView > 10*modifier )
        setProperties(  _fieldOfView - 10*modifier,
                        _nearClippingPlane,
                        _farClippingPlane   );
    else
        setProperties(0.0);
    

    updateTransformationMatrix();
}


void Camera::translate( const glm::vec3& translation )
{
    setPose(    _cameraPosition + translation,
                _cameraDirection,
                _cameraUp );
    updateTransformationMatrix();
}


void Camera::rotate(    GLfloat radians,
                        const glm::vec3& axis,
                        const glm::vec3& center )
{
    auto transform      = glm::translate(center) * glm::rotate(radians, axis) * glm::translate(-center);
    auto pos            = transform * glm::vec4( _cameraPosition, 1.0f );
    auto dir            = transform * glm::vec4( _cameraDirection, 1.0f );
    auto up             = transform * glm::vec4( _cameraUp, 1.0f );
    
    setPose( glm::vec3(pos), glm::vec3(dir), glm::vec3(up) );
    updateTransformationMatrix();
}


void Camera::setPose(   const glm::vec3& position,
                        const glm::vec3& direction,
                        const glm::vec3& cameraUp )
{
    _cameraPosition     = position;
    _cameraDirection    = glm::normalize(direction);
    _cameraUp           = glm::normalize(cameraUp);

    // Compute view tensor
    _viewMatrix = glm::lookAt(  _cameraPosition,
                                _cameraPosition + _cameraDirection,
                                _cameraUp );
}



void Camera::setPose(   const glm::vec3& position,
                        const glm::vec3& direction )
{
    glm::vec3 dir = glm::normalize(direction);

    // Compute Euler angles
    //  phi     : rotation around the original z axis
    //  theta   : rotation around the modified y axis
    GLfloat theta   = glm::acos( dir[2] );
    GLfloat phi     = (GLfloat)std::atan2( dir[1], dir[0] );

    // Compute camera up vector
    glm::vec3 yRotated  = glm::rotate(  glm::vec3( 0.0f, 1.0f, 0.0f ),
                                        phi,
                                        glm::vec3( 0.0f, 0.0f, 1.0f )   );
    glm::vec3 cameraUp  = glm::vec3( 0.0f, 0.0f, 1.0f );
    cameraUp            = glm::rotate(  cameraUp,
                                        -theta,
                                        yRotated    );
                                        
    setPose( position, direction, cameraUp );
}


void Camera::setProperties( GLfloat fieldOfView,
                            GLfloat nearClippingPlane,
                            GLfloat farClippingPlane )
{
    _fieldOfView        = fieldOfView;
    _nearClippingPlane  = nearClippingPlane;
    _farClippingPlane   = farClippingPlane;

    if ( _fieldOfView != 0.0 )
        _projectionMatrix = glm::perspective(   glm::radians(_fieldOfView),
                                                (GLfloat)_width / (GLfloat)_height,
                                                _nearClippingPlane,
                                                _farClippingPlane );
    else
        _projectionMatrix = glm::ortho( -_width/2.0f,
                                        _width/2.0f,
                                        -_height/2.0f,
                                        _height/2.0f,
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
                _cameraDirection,
                _cameraUp );
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


glm::vec3 Camera::screenToWorld( double x, double y ) const
{
    // Get window size
    int w, h;
    glfwGetFramebufferSize( _context->window(), &w, &h );

    // Get depth buffer value
    GLfloat z = 0.0f;
    glReadPixels(   x,y,
                    1,1,
                    GL_DEPTH_COMPONENT,
                    GL_FLOAT,
                    &z );

    // Map to [-1,1]
    x                   = 2.0 * x/(double)w - 1.0;
    y                   = 2.0 * y/(double)h - 1.0;
    z                   = 2.0 * z - 1.0;

    // Transform to world
    glm::vec4 world     = glm::inverse(_transformationMatrix) * glm::vec4( x, y, (GLfloat)z, 1.0f );        
    return glm::vec3(world) / world[3];
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


const glm::vec3& Camera::cameraUp() const
{
    return _cameraUp;
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



InteractiveCamera::InteractiveCamera( GLContext& context, const std::string& className ) :
    Camera( context, className ),
    _mousePressPosition( 0.0f, 0.0f, 0.0f ),
    _cursorPosition( 0.0f, 0.0f, 0.0f )
{
}


InteractiveCamera::InteractiveCamera( const InteractiveCamera& copy ):
    Camera( copy ),
    _mousePressPosition( copy._mousePressPosition ),
    _cursorPosition( copy._cursorPosition )
{
}


void InteractiveCamera::setMousePressPosition( double x, double y )
{
    _mousePressPosition = screenToWorld( x, y );
}


void InteractiveCamera::setMousePressPosition( const glm::vec3& pos )
{
    _mousePressPosition = pos;
}


void InteractiveCamera::setCursorPosition( double x, double y )
{
    _cursorPosition = screenToWorld( x, y );
}


void InteractiveCamera::setCursorPosition( const glm::vec3& pos )
{
    _cursorPosition = pos;
}


const glm::vec3& InteractiveCamera::mousePressPosition( ) const
{
    return _mousePressPosition;
}


const glm::vec3& InteractiveCamera::cursorPosition( ) const
{
    return _cursorPosition;
}


ArcballCamera::ArcballCamera(   GLContext& context,
                                const std::string& className ) :
    InteractiveCamera( context, className ),
    _center( 0.0f, 0.0f, 0.0f )
{
}


void ArcballCamera::translate( const glm::vec3& translation )
{
}


void ArcballCamera::rotate( GLfloat radians,
                            const glm::vec3& axis,
                            const glm::vec3& centerPoint )
{
    InteractiveCamera::rotate( radians, axis, _center );
}


void ArcballCamera::setCenter( const glm::vec3& point )
{
    _center = point;
    setPose(    _cameraPosition, 
                glm::normalize(_center - _cameraPosition) );
}


const glm::vec3& ArcballCamera::center() const
{
    return _center;
}


}
}