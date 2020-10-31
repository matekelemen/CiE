#ifndef CIEGL_CAMERA_HPP
#define CIEGL_CAMERA_HPP

// --- Internal Imports ---
#include "ciegl/packages/rigidbody/inc/RigidBody.hpp"

// --- External Imports ---
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// --- Utility Includes ---
#include "cieutils/packages/logging/inc/Loggee.hpp"

// --- STD Imports ---
#include <memory>

namespace cie::gl {


class Camera;
class InteractiveCamera;
using CameraPtr             = std::shared_ptr<Camera>;
using InteractiveCameraPtr  = std::shared_ptr<InteractiveCamera>;



class Camera :
    public utils::Loggee,
    public RigidBody
{
public:
    Camera( utils::Logger& r_logger,
            const std::string& r_name = "Camera" );
    Camera( Camera& camera );
    Camera& operator=( Camera& camera );
    ~Camera();

    virtual void zoom( double modifier );
    virtual void translate( const glm::dvec3& translation ) override;
    virtual void rotate(    double angles,
                            const glm::dvec3& axis,
                            const glm::dvec3& center ) override;

    void setPose(   const glm::dvec3& position,
                    const glm::dvec3& direction,
                    const glm::dvec3& up    ) override;
    void setProperties( double fieldOfView,
                        double nearClippingPlane,
                        double farClippingPlane );
    void setProperties( double fieldOfView );

    void updateViewMatrix();
    void updateTransformationMatrix();
    void update();

    const glm::mat4& viewMatrix() const;
    const glm::mat4& projectionMatrix() const;
    const glm::mat4& transformationMatrix() const;
    glm::dvec3 screenToWorld( double x, double y ) const;

    double fieldOfView() const;
    double& width();
    double& height();
    double nearClippingPlane() const;
    double farClippingPlane() const;

protected:
    double      _fieldOfView;
    double      _width;
    double      _height;
    double      _nearClippingPlane;
    double      _farClippingPlane;

    glm::mat4   _viewMatrix;
    glm::mat4   _projectionMatrix;
    glm::mat4   _transformationMatrix;
};



class InteractiveCamera : public Camera
{
public:
    InteractiveCamera(  utils::Logger& r_logger,
                        const std::string& r_name = "InteractiveCamera" );
    InteractiveCamera( InteractiveCamera& copy );
    InteractiveCamera& operator=( InteractiveCamera& copy ) = default;

    void setMousePressPosition( double x, double y );
    void setMousePressPosition( const glm::dvec3& pos );

    void setCursorPosition( double x, double y );
    void setCursorPosition( const glm::dvec3& pos );

    const glm::dvec3& mousePressPosition() const;
    const glm::dvec3& cursorPosition() const;

protected:
    glm::dvec3   _mousePressPosition;
    glm::dvec3   _cursorPosition;
};


class ArcballCamera : public InteractiveCamera
{
public:
    ArcballCamera(  utils::Logger& r_logger,
                    const std::string& r_name = "ArcballCamera" );

    void rotate(    double radians,
                    const glm::dvec3& axis,
                    const glm::dvec3& centerPoint = glm::dvec3(0.0,0.0,0.0) ) override;

    void setCenter( const glm::dvec3& point );
    const glm::dvec3& center() const;

protected:
    glm::dvec3   _center;
};


} // namespace cie::gl

#endif