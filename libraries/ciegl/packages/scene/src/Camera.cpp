// --- External Includes ---
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/rotate_vector.hpp>

// --- Internal Includes ---
#include "ciegl/packages/scene/inc/Camera.hpp"

// --- STL Includes ---
#include <cmath>

namespace cie::gl {


Camera::Camera( utils::Logger& r_logger, const std::string& r_name ) :
    utils::Loggee( r_logger, r_name ),
    RigidBody(),
    _fieldOfView( 90.0 ),
    _nearClippingPlane( 0.1 ),
    _farClippingPlane( 10.0 ),
    _viewMatrix( 1.0 ),
    _projectionMatrix( 1.0 ),
    _transformationMatrix( 1.0 )
{
    // Update width and height (orthographic mode)
    //const auto& windowSize = _r_context.window()->getSize();

    _width      = 2.0;
    _height     = 2.0 * 600.0 / 800.0;
    update();
}


Camera::Camera( Camera& r_copy ) :
    utils::Loggee( r_copy.logger(), r_copy.name() ),
    RigidBody( r_copy._position, r_copy._direction, r_copy._up ),
    _fieldOfView( r_copy._fieldOfView ),
    _width( r_copy._width ),
    _height( r_copy._height ),
    _nearClippingPlane( r_copy._nearClippingPlane ),
    _farClippingPlane( r_copy._farClippingPlane ),
    _viewMatrix( r_copy._viewMatrix ),
    _projectionMatrix( r_copy._projectionMatrix ),
    _transformationMatrix( r_copy._transformationMatrix )
{
    update();
}


Camera& Camera::operator=( Camera& r_copy )
{
    _position               = r_copy._position;
    _direction              = r_copy._direction;
    _up                     = r_copy._up;
    _fieldOfView            = r_copy._fieldOfView;
    _width                  = r_copy._width;
    _height                 = r_copy._height;
    _nearClippingPlane      = r_copy._nearClippingPlane;
    _farClippingPlane       = r_copy._farClippingPlane;
    _viewMatrix             = r_copy._viewMatrix;
    _projectionMatrix       = r_copy._projectionMatrix;
    _transformationMatrix   = r_copy._transformationMatrix;
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
    //const auto& windowSize = _r_context.window()->getSize();

    // Get depth buffer value
    double z = 0.0;
    glReadPixels(   x,y,
                    1,1,
                    GL_DEPTH_COMPONENT,
                    GL_FLOAT,
                    &z );

    // Map to [-1,1]
    x                   = 2.0 * x/800.0 - 1.0;
    y                   = 2.0 * y/600.0 - 1.0;
    z                   = 2.0 * z - 1.0;

    // Transform to world
    glm::dvec4 world     = glm::inverse(_transformationMatrix) * glm::dvec4( x, y, z, 1.0 );        
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



InteractiveCamera::InteractiveCamera( utils::Logger& r_logger, const std::string& r_name ) :
    Camera( r_logger, r_name ),
    _mousePressPosition( 0.0, 0.0, 0.0 ),
    _cursorPosition( 0.0, 0.0, 0.0 )
{
}


InteractiveCamera::InteractiveCamera( InteractiveCamera& copy ):
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


ArcballCamera::ArcballCamera(   utils::Logger& r_logger,
                                const std::string& r_name ) :
    InteractiveCamera( r_logger, r_name ),
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