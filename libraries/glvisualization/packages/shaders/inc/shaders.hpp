#ifndef GLVISUALIZATION_SHADERS_HPP
#define GLVISUALIZATION_SHADERS_HPP

// --- External Includes ---
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// --- Utility Includes ---
#include "cmake_variables.hpp"

// --- STD Includes ---
#include <string>
#include <vector>
#include <iostream>
#include <ostream>

namespace cie {
namespace gl {


using ShaderCode                = std::string;
const std::string SHADER_PATH   = DATA_PATH + "/glvisualization/shaders";


struct ShaderStruct
{
    ShaderStruct(   const ShaderCode& source,
                    const std::vector<std::string>& attributes,
                    const std::vector<GLuint>& sizes,
                    const std::vector<GLuint>& strides,
                    const std::vector<GLuint>& offsets,
                    const std::vector<std::string>& uniforms,
                    const std::vector<std::string>& textures );
    ShaderStruct( const std::string& fileName );
    ShaderStruct( const ShaderStruct& copy );
    ShaderStruct operator=( const ShaderStruct& copy );
    ShaderStruct( );
    void print( std::ostream& stream = std::cout ) const;
    
    ShaderCode                  _source;
    std::vector<std::string>    _attributes;
    std::vector<GLuint>         _sizes;
    std::vector<GLuint>         _strides;
    std::vector<GLuint>         _offsets;
    std::vector<std::string>    _uniforms;
    std::vector<std::string>    _textures;
};


}
}

#endif