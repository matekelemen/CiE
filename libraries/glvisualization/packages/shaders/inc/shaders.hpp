#ifndef GLVISUALIZATION_SHADERS_HPP
#define GLVISUALIZATION_SHADERS_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

namespace cie {
namespace gl {


using ShaderCode = std::string;


struct ShaderStruct
{
    ShaderStruct(   const ShaderCode& source,
                    const std::vector<std::string>& attributes,
                    const std::vector<GLuint>& sizes,
                    const std::vector<GLuint>& strides,
                    const std::vector<GLuint>& offsets,
                    const std::vector<std::string>& uniforms );
    ShaderStruct( const ShaderStruct& copy );
    ShaderStruct operator=( const ShaderStruct& copy );
    ShaderStruct( );
    
    ShaderCode                  _source;
    std::vector<std::string>    _attributes;
    std::vector<GLuint>         _sizes;
    std::vector<GLuint>         _strides;
    std::vector<GLuint>         _offsets;
    std::vector<std::string>    _uniforms;
};


}
}

#include "defaultShaders.hpp"
#include "defaultColorShaders.hpp"

#endif