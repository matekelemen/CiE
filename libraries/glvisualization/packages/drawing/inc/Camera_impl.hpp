#ifndef GLVISUALIZATION_CAMERA_IMPL_HPP
#define GLVISUALIZATION_CAMERA_IMPL_HPP

#include <iostream>

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
        _cameraDirection    = direction;
    }

    _viewMatrix = glm::lookAt(  _cameraPosition,
                                _cameraPosition + _cameraDirection,
                                glm::vec3( {0.0f, 1.0f, 0.0f} ) );
    
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