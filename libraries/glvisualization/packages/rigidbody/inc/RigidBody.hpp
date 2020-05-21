#ifndef GLVISUALIZATION_RIGID_BODY_HPP
#define GLVISUALIZATION_RIGID_BODY_HPP

// --- External Includes ---
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>


namespace cie {
namespace gl {


class RigidBody
{
public:
    RigidBody(  const glm::dvec3& position,
                const glm::dvec3& direction,
                const glm::dvec3& up );
    RigidBody();

    const glm::dvec3& position() const;
    const glm::dvec3& direction() const;
    const glm::dvec3& up() const;
    const glm::dquat& quaternion() const;

    virtual void setPosition(   const glm::dvec3& position );
    virtual void setPose(   const glm::dvec3& position,
                            const glm::dvec3& direction,
                            const glm::dvec3& up );

    virtual void translate( const glm::dvec3& translation );
    virtual void rotate(    double radians,
                            const glm::dvec3& axis,
                            const glm::dvec3& center  );
    //void rotateLocal(   double yawRadians,
    //                    double pitchRadians,
    //                    double rollRadians );
    //void rotateGlobal(  double zRadians,
    //                    double yRadians,
    //                    double xRadians );

protected:
    glm::dvec3   _position;
    glm::dvec3   _direction;
    glm::dvec3   _up;
    glm::dquat   _quaternion;
};


}
}


#endif