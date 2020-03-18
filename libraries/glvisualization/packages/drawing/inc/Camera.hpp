#ifndef GLVISUALIZATION_CAMERA_HPP
#define GLVISUALIZATION_CAMERA_HPP

// --- Internal Imports ---
#include "../../context/inc/AbsContextClass.hpp"

// --- External Imports ---
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// --- STD Imports ---
#include <memory>

namespace cie {
namespace gl {


class Camera;
class InteractiveCamera;
using CameraPtr             = std::shared_ptr<Camera>;
using InteractiveCameraPtr  = std::shared_ptr<InteractiveCamera>;



class Camera : public AbsContextClass
{
public:
    Camera( GLContext& context );
    Camera( const Camera& camera );
    Camera& operator=( const Camera& camera );
    ~Camera();

    virtual void zoom( GLfloat modifier );
    virtual void translate( const glm::vec3& translation );
    virtual void rotate(    GLfloat radians,
                            const glm::vec3& axis,
                            const glm::vec3& center );

    void setPose(   const glm::vec3& position,
                    const glm::vec3& direction,
                    const glm::vec3& cameraUp );

    void setPose(   const glm::vec3& position,
                    const glm::vec3& direction );

    void setProperties( GLfloat fieldOfView,
                        GLfloat nearClippingPlane,
                        GLfloat farClippingPlane );
    void setProperties( GLfloat fieldOfView );

    void updateTransformationMatrix();
    void update();

    const glm::mat4& viewMatrix() const;
    const glm::mat4& projectionMatrix() const;
    const glm::mat4& transformationMatrix() const;
    glm::vec3 screenToWorld( double x, double y ) const;

    const glm::vec3& cameraPosition() const;
    const glm::vec3& cameraDirection() const;
    const glm::vec3& cameraUp() const;
    GLfloat fieldOfView() const;
    GLfloat& width();
    GLfloat& height();
    GLfloat nearClippingPlane() const;
    GLfloat farClippingPlane() const;

protected:
    glm::vec3   _cameraPosition;
    glm::vec3   _cameraDirection;
    glm::vec3   _cameraUp;
    GLfloat     _fieldOfView;
    GLfloat     _width;
    GLfloat     _height;
    GLfloat     _nearClippingPlane;
    GLfloat     _farClippingPlane;

    glm::mat4   _viewMatrix;
    glm::mat4   _projectionMatrix;
    glm::mat4   _transformationMatrix;
};



class InteractiveCamera : public Camera
{
public:
    InteractiveCamera( GLContext& context );
    InteractiveCamera( const InteractiveCamera& copy );
    InteractiveCamera& operator=( const InteractiveCamera& copy ) = default;

    void setMousePressPosition( double x, double y );
    void setMousePressPosition( const glm::vec3& pos );

    void setCursorPosition( double x, double y );
    void setCursorPosition( const glm::vec3& pos );

    const glm::vec3& mousePressPosition() const;
    const glm::vec3& cursorPosition() const;

protected:
    glm::vec3   _mousePressPosition;
    glm::vec3   _cursorPosition;
};


}
}

#endif