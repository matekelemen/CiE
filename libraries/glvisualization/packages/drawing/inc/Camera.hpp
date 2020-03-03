#ifndef GLVISUALIZATION_CAMERA_HPP
#define GLVISUALIZATION_CAMERA_HPP

// --- Internal Imports ---
#include "../../context/inc/AbsContextClass.hpp"

// --- External Imports ---
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace cie {
namespace gl {


class Camera : public AbsContextClass
{
public:
    Camera( GLContext& context );

    template <class ContainerType>
    void setCameraPose( const ContainerType& position,
                        const ContainerType& direction );

    void setCameraProperties(   float fieldOfView,
                                float nearClippingPlane,
                                float farClippingPlane );

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

private:
    glm::mat4   _viewMatrix;
    glm::mat4   _projectionMatrix;
    glm::mat4   _transformationMatrix;
};


}
}

#include "Camera_impl.hpp"

#endif