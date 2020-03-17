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
    virtual void rotate(    GLfloat degrees,
                            const glm::vec3& axis );

    template <class ContainerType>
    void setPose(   const ContainerType& position,
                    const ContainerType& direction );

    void setProperties( GLfloat fieldOfView,
                        GLfloat nearClippingPlane ,
                        GLfloat farClippingPlane );
    void setProperties( GLfloat fieldOfView );

    void updateTransformationMatrix();
    void update();

    const glm::mat4& viewMatrix() const;
    const glm::mat4& projectionMatrix() const;
    const glm::mat4& transformationMatrix() const;

    const glm::vec3& cameraPosition() const;
    const glm::vec3& cameraDirection() const;
    GLfloat fieldOfView() const;
    GLfloat& width();
    GLfloat& height();
    GLfloat nearClippingPlane() const;
    GLfloat farClippingPlane() const;

protected:
    glm::vec3   _cameraPosition;
    glm::vec3   _cameraDirection;
    GLfloat     _fieldOfView;
    GLfloat     _width;
    GLfloat     _height;
    GLfloat     _nearClippingPlane;
    GLfloat     _farClippingPlane;

    glm::mat4   _viewMatrix;
    glm::mat4   _projectionMatrix;
    glm::mat4   _transformationMatrix;
};





//class Camera2D : public Camera
//{
//public:
//    Camera2D( GLContext& context );
//    Camera2D( const Camera2D& copy );
//    Camera2D& operator=( const Camera2D& copy ) = default;
//
//protected:
//    glm::vec3   _normal;
//};



class InteractiveCamera : public Camera
{
public:
    InteractiveCamera( GLContext& context );
    InteractiveCamera( const InteractiveCamera& copy );
    InteractiveCamera& operator=( const InteractiveCamera& copy ) = default;

    glm::vec3 screenToWorld( double x, double y ) const;
    void setMousePressPosition( double x, double y );
    void setMousePressPosition( const glm::vec3& mousePos );
    const glm::vec3& mousePressPosition( ) const;

protected:
    glm::vec3   _mousePressPosition;
};


}
}

#include "Camera_impl.hpp"

#endif