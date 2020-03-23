// --- Internal Includes ---
#include "../inc/shaders.hpp"
#include "file.hpp"

// --- STD Includes ---
#include <string>
#include <sstream>

namespace cie {
namespace gl {


ShaderStruct::ShaderStruct( const ShaderCode& source,
                            const std::vector<std::string>& attributes,
                            const std::vector<GLuint>& sizes,
                            const std::vector<GLuint>& strides,
                            const std::vector<GLuint>& offsets,
                            const std::vector<std::string>& uniforms,
                            const std::vector<std::string>& textures ) :
    _source(source),
    _attributes(attributes),
    _sizes(sizes),
    _strides(strides),
    _offsets(offsets),
    _uniforms(uniforms),
    _textures(textures)
{
}


ShaderStruct::ShaderStruct( const std::string& shaderFileName )
{
    // Open shader file
    FileManager manager( SHADER_PATH );
    auto file = manager.open( shaderFileName );

    // Read contents
    std::stringstream stream;
    stream << file.rdbuf();
    // TODO
}


ShaderStruct::ShaderStruct( const ShaderStruct& copy ) :
    _source(copy._source),
    _attributes(copy._attributes),
    _sizes(copy._sizes),
    _strides(copy._strides),
    _offsets(copy._offsets),
    _uniforms(copy._uniforms),
    _textures(copy._textures)
{
}


ShaderStruct ShaderStruct::operator=( const ShaderStruct& copy )
{
    _source     = copy._source;
    _attributes = copy._attributes;
    _sizes      = copy._sizes;
    _strides    = copy._strides;
    _offsets    = copy._offsets;
    _uniforms   = copy._uniforms;
    _textures   = copy._textures;
    return *this;
}


ShaderStruct::ShaderStruct()
{
}



}
}