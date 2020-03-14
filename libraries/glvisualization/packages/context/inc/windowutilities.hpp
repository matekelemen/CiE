#ifndef GLVISUALIZATION_WINDOW_UTILITIES_HPP
#define GLVISUALIZATION_WINDOW_UTILITIES_HPP

// --- External Includes ---
#include <GLFW/glfw3.h>

// --- STD Includes ---
#include <utility>
#include <cstddef>

namespace cie {
namespace gl {


std::pair<size_t, size_t> getMonitorResolution( GLFWmonitor* monitor=glfwGetPrimaryMonitor() );
std::pair<size_t, size_t> getPrimaryMonitorResolution();


}
}


#endif