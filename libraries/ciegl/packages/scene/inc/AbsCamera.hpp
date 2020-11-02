#ifndef CIE_GL_ABS_CAMERA_HPP
#define CIE_GL_ABS_CAMERA_HPP

// --- External Includes ---
#include <glm/glm.hpp>

// --- Internal Includes ---
#include "ciegl/packages/rigidbody/inc/RigidBody.hpp"


namespace cie::gl {


class AbsCamera
{
public:
    using internal_matrix_type = glm::mat4;
    using internal_vector_type = glm::vec4;
    using external_vector_type = glm::dvec3;

protected:
    internal_matrix_type    _viewMatrix;
    internal_matrix_type    _projectionMatrix;
    internal_matrix_type    _transformationMatrix;
};


} // namespace cie::gl

#endif