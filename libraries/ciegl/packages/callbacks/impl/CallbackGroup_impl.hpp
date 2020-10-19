#ifndef CIEGL_CALLBACK_GROUP_IMPL_HPP
#define CIEGL_CALLBACK_GROUP_IMPL_HPP

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- Internal Includes ---
#include "ciegl/packages/context/inc/GLFWWindow.hpp"

// --- STL Includes ---
#include <sstream>

namespace cie::gl {
    

template <class CallbackType>
CallbackType makeCallback(  CallbackType callback,
                            DrawManager* drawManager )
{
    GLFWwindow* p_window = detail::getGLFWwindow( drawManager->context().window() );
    if ( glfwGetWindowUserPointer(p_window) == nullptr )
        glfwSetWindowUserPointer( p_window, static_cast<void*>(drawManager) );
    return callback;
}


template <class CameraType>
CameraType* CallbackGroup::getCameraPtr( WindowPtr p_window )
{
    // Get window user pointer
    GLFWwindow* p_rawWindow = detail::getGLFWwindow(p_window);
    void* userPtr           = glfwGetWindowUserPointer( p_rawWindow );

    // Check if window user pointer was set
    if (userPtr == nullptr)
    {
        std::stringstream stream;
        stream << userPtr;
        CIE_THROW( std::runtime_error, "Unset user pointer for window " + stream.str() )
    }

    // Reinterpret as DrawManager and get camera
    DrawManager* manager    = static_cast<DrawManager*>(glfwGetWindowUserPointer( p_rawWindow ));
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