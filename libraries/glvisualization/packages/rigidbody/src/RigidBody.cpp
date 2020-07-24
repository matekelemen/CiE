// --- External Includes ---
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>

// --- Internal Includes ---
#include "../inc/RigidBody.hpp"

// --- STL Includes ---
#include <iostream>

namespace cie {
namespace gl {


RigidBody::RigidBody(   const glm::dvec3& position,
                        const glm::dvec3& direction,
                        const glm::dvec3& up ) :
    _position( position ),
    _direction( direction ),
    _up( up ),
    _quaternion( 1.0f, 0.0f, 0.0f, 0.0f )
{
}


RigidBody::RigidBody() :
    RigidBody(  glm::vec3( 0.0f, 0.0f, 0.0f ),
                glm::vec3( 1.0f, 0.0f, 0.0f ),
                glm::vec3( 0.0f, 0.0f, 1.0f ) )
{
}


const glm::dvec3& RigidBody::position() const
{
    return _position;
}


const glm::dvec3& RigidBody::direction() const
{
    return _direction;
}


const glm::dvec3& RigidBody::up() const
{
    return _up;
}


const glm::dquat& RigidBody::quaternion() const
{
    return _quaternion;
}


void RigidBody::setPosition( const glm::dvec3& position )
{
    _position = position;
}


void RigidBody::setPose(    const glm::dvec3& position,
                            const glm::dvec3& direction,
                            const glm::dvec3& up )
{
    _position   = position;
    _direction  = glm::normalize( direction );
    _up         = glm::normalize( up );
}


void RigidBody::translate( const glm::dvec3& translation )
{
    _position += translation;
}


void RigidBody::rotate( double radians,
                        const glm::dvec3& axis,
                        const glm::dvec3& center )
{
    // Create rotation tensor
    glm::dmat4 rotation = glm::rotate( radians, axis );

    // Rotate members
    _position           = glm::translate(center) * rotation * glm::translate(-center) * glm::dvec4( _position, 1.0 );
    _direction          = glm::dvec3( rotation * glm::dvec4(_direction, 1.0) );
    _up                 = glm::dvec3( rotation * glm::dvec4(_up, 1.0) );
}





}
}