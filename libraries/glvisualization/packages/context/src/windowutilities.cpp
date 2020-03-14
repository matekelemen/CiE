// --- External Includes ---
#include <GLFW/glfw3.h>

// --- Internal Includes ---
#include "../inc/windowutilities.hpp"

namespace cie {
namespace gl {


std::pair<size_t, size_t> getMonitorResolution( GLFWmonitor* monitor )
{
    const GLFWvidmode* mode = glfwGetVideoMode( monitor );
    return std::make_pair<size_t, size_t>( mode->width, mode->height );
}


std::pair<size_t, size_t> getPrimaryMonitorResolution()
{
    const GLFWvidmode* mode = glfwGetVideoMode( glfwGetPrimaryMonitor() );
    return std::make_pair<size_t, size_t>( mode->width, mode->height );
}


}
}