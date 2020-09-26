#ifndef GLVISUALIZATION_CALLBACK_GROUP_IMPL_HPP
#define GLVISUALIZATION_CALLBACK_GROUP_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- STL Includes ---
#include <sstream>

namespace cie::gl {
    

template <class CallbackType>
CallbackType makeCallback(  CallbackType callback,
                            DrawManager* drawManager )
{
    if ( glfwGetWindowUserPointer( drawManager->context()->window()) == nullptr )
        glfwSetWindowUserPointer( drawManager->context()->window(), static_cast<void*>(drawManager) );
    return callback;
}


template <class CameraType>
CameraType* CallbackGroup::getCameraPtr( WindowPtr window )
{
    // Get window user pointer
    void* userPtr           = glfwGetWindowUserPointer(window);

    // Check if window user pointer was set
    if (userPtr == nullptr)
    {
        std::stringstream stream;
        stream << userPtr;
        CIE_THROW( std::runtime_error, "Unset user pointer for window " + stream.str() )
    }

    // Reinterpret as DrawManager and get camera
    DrawManager* manager    = static_cast<DrawManager*>(glfwGetWindowUserPointer(window));
    CameraType* camera      = nullptr;

    // Try casting the camera to the requested type
    try
    {
        camera = static_cast<CameraType*>( manager->camera().get() );
    }
    catch(const std::exception& err)
    {
        std::cerr << "Camera casting failed! Thrown error:\n" << err.what() << '\n';
    }
    

    return camera;
}


} // namespace cie::gl

#endif