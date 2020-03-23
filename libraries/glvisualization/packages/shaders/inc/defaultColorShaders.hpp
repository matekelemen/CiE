#ifndef GLVISUALIZATION_DEFAULT_COLOR_SHADERS
#define GLVISUALIZATION_DEFAULT_COLOR_SHADERS

#include "shaders.hpp"

namespace cie {
namespace gl {

extern const ShaderStruct defaultColorVertexShader;
extern const ShaderStruct defaultColorGeometryShader;
extern const ShaderStruct defaultColorFragmentShader;

}
}

#include "defaultColorShaders_impl.hpp"

#endif