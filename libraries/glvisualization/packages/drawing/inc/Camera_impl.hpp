#ifndef GLVISUALIZATION_CAMERA_IMPL_HPP
#define GLVISUALIZATION_CAMERA_IMPL_HPP

#include <iostream>
#include <glm/gtx/rotate_vector.hpp>

namespace cie {
namespace gl {


template <class ContainerType>
void Camera::setCameraPose(  const ContainerType& position,
                            const ContainerType& direction )
{
    if ( typeid(ContainerType) != typeid(glm::vec3) )
    {
        _cameraPosition     = glm::vec3( position[0], position[1], position[2] );
        _cameraDirection    = glm::vec3( direction[0], direction[1], direction[2] );
    }
    else
    {
        _cameraPosition     = position;
        _cameraDirection    = glm::normalize(direction);
    }

    // Compute Euler angles
    //  phi     : rotation around the original z axis
    //  theta   : rotation around the modified y axis
    GLfloat theta   = glm::acos( _cameraDirection[0]*_cameraDirection[0] );
    GLfloat phi     = glm::asin( _cameraDirection[0] / glm::cos(theta) );

    // Compute camera up vector
    glm::vec3 yRotated  = glm::rotate(  glm::vec3( 0.0f, 1.0f, 0.0f ),
                                        phi,
                                        glm::vec3( 1.0f, 0.0f, 0.0f )   );
    glm::vec3 cameraUp  = glm::rotate(  glm::vec3( 0.0f, 0.0f, 1.0f ),
                                        phi,
                                        glm::vec3( 1.0f, 0.0f, 0.0f )   );
    cameraUp            = glm::rotate(  cameraUp,
                                        theta,
                                        yRotated    );

    // Compute view tensor
    _viewMatrix = glm::lookAt(  _cameraPosition,
                                _cameraPosition + _cameraDirection,
                                cameraUp );
    
    //for (size_t i=0; i<4; ++i)
    //    {
    //        for (size_t j=0; j<4; ++j)
    //            std::cout << std::to_string( _viewMatrix[i][j] ) + ", ";
    //        std::cout << "\n";
    //    }
}


}
}

#endif