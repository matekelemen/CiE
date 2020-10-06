// --- Internal Includes ---
#include "ciegl/packages/drawing/inc/Camera.hpp"

// --- External Includes ---
#include <glm/gtx/rotate_vector.hpp>

// --- STL Includes ---
#include <cmath>

namespace cie::gl {


Camera::Camera( GLContext& context, const std::string& className ) :
    AbsContextClass( context, className ),
    RigidBody(),
    _fieldOfView( 90.0 ),
    _nearClippingPlane( 0.1 ),
    _farClippingPlane( 10.0 ),
    _viewMatrix( 1.0 ),
    _projectionMatrix( 1.0 ),
    _transformationMatrix( 1.0 )
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


Camera::Camera( const Camera& copy ) :
    AbsContextClass( *copy._context, "Camera" ),
    RigidBody( copy._position, copy._direction, copy._up ),
    _fieldOfView( copy._fieldOfView ),
    _width( copy._width ),
    _height( copy._height ),
    _nearClippingPlane( copy._nearClippingPlane ),
    _farClippingPlane( copy._farClippingPlane ),
    _viewMatrix( copy._viewMatrix ),
    _projectionMatrix( copy._projectionMatrix ),
    _transformationMatrix( copy._transformationMatrix )
{
    update();
}


Camera& Camera::operator=( const Camera& copy )
{
    _position               = copy._position;
    _direction              = copy._direction;
    _up                     = copy._up;
    _fieldOfView            = copy._fieldOfView;
    _width                  = copy._width;
    _height                 = copy._height;
    _nearClippingPlane      = copy._nearClippingPlane;
    _farClippingPlane       = copy._farClippingPlane;
    _viewMatrix             = copy._viewMatrix;
    _projectionMatrix       = copy._projectionMatrix;
    _transformationMatrix   = copy._transformationMatrix;
    update();

    return *this;
}


Camera::~Camera()
{
}


void Camera::zoom( double modifier )
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


void Camera::translate( const glm::dvec3& translation )
{
    RigidBody::translate(translation);
    updateViewMatrix();
    updateTransformationMatrix();
}


void Camera::rotate(    double angles,
                        const glm::dvec3& axis,
                        const glm::dvec3& center )
{
    RigidBody::rotate( angles, axis, center );
    updateViewMatrix();
    updateTransformationMatrix();
}


void Camera::setPose(   const glm::dvec3& position,
                        const glm::dvec3& direction,
                        const glm::dvec3& up  )
{
    RigidBody::setPose( position, direction, up );
    updateViewMatrix();
}


void Camera::setProperties( double fieldOfView,
                            double nearClippingPlane,
                            double farClippingPlane )
{
    _fieldOfView        = fieldOfView;
    _nearClippingPlane  = nearClippingPlane;
    _farClippingPlane   = farClippingPlane;

    if ( _fieldOfView != 0.0 )
        _projectionMatrix = glm::perspective(   glm::radians(_fieldOfView),
                                                (double)_width / (double)_height,
                                                _nearClippingPlane,
                                                _farClippingPlane );
    else
        _projectionMatrix = glm::ortho( -_width/2.0,
                                        _width/2.0,
                                        -_height/2.0,
                                        _height/2.0,
                                        _nearClippingPlane,
                                        _farClippingPlane   );
}


void Camera::setProperties( double fieldOfView )
{
    setProperties(  fieldOfView,
                    _nearClippingPlane,
                    _farClippingPlane   );
}


void Camera::updateViewMatrix()
{
    _viewMatrix = glm::lookAt(  _position,
                                _position + _direction,
                                _up );
}


void Camera::updateTransformationMatrix()
{
    _transformationMatrix = _projectionMatrix * _viewMatrix;
}


void Camera::update()
{
    setPose(    _position,
                _direction,
                _up );
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


glm::dvec3 Camera::screenToWorld( double x, double y ) const
{
    // Get window size
    int w, h;
    glfwGetFramebufferSize( _context->window(), &w, &h );

    // Get depth buffer value
    double z = 0.0;
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
    glm::dvec4 world     = glm::inverse(_transformationMatrix) * glm::dvec4( x, y, (double)z, 1.0 );        
    return glm::dvec3(world) / world[3];
}


double& Camera::width()
{
    return _width;
}


double& Camera::height()
{
    return _height;
}


double Camera::fieldOfView() const
{
    return _fieldOfView;
}


double Camera::nearClippingPlane() const
{
    return _nearClippingPlane;
}


double Camera::farClippingPlane() const
{
    return _farClippingPlane;
}



InteractiveCamera::InteractiveCamera( GLContext& context, const std::string& className ) :
    Camera( context, className ),
    _mousePressPosition( 0.0, 0.0, 0.0 ),
    _cursorPosition( 0.0, 0.0, 0.0 )
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


void InteractiveCamera::setMousePressPosition( const glm::dvec3& pos )
{
    _mousePressPosition = pos;
}


void InteractiveCamera::setCursorPosition( double x, double y )
{
    _cursorPosition = screenToWorld( x, y );
}


void InteractiveCamera::setCursorPosition( const glm::dvec3& pos )
{
    _cursorPosition = pos;
}


const glm::dvec3& InteractiveCamera::mousePressPosition( ) const
{
    return _mousePressPosition;
}


const glm::dvec3& InteractiveCamera::cursorPosition( ) const
{
    return _cursorPosition;
}


ArcballCamera::ArcballCamera(   GLContext& context,
                                const std::string& className ) :
    InteractiveCamera( context, className ),
    _center( 0.0, 0.0, 0.0 )
{
}


void ArcballCamera::rotate( double radians,
                            const glm::dvec3& axis,
                            const glm::dvec3& centerPoint )
{
    InteractiveCamera::rotate( radians, axis, _center );
}


void ArcballCamera::setCenter( const glm::dvec3& point )
{
    _center = point;
    setPose(    _position, 
                glm::normalize(_center - _position),
                _up );
}


const glm::dvec3& ArcballCamera::center() const
{
    return _center;
}


} // namespace cie::gl