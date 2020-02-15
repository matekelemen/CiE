#ifndef GLVISUALIZATION_SHADERS_IMPL_HPP
#define GLVISUALIZATION_SHADERS_IMPL_HPP

#include "../inc/shaders.hpp"

namespace cie {
namespace gl {


ShaderStruct::ShaderStruct( const ShaderCode& source,
                            const std::vector<std::string>& attributes,
                            const std::vector<GLuint>& sizes,
                            const std::vector<GLuint>& strides,
                            const std::vector<GLuint>& offsets,
                            const std::vector<std::string>& uniforms ) :
    _source(source),
    _attributes(attributes),
    _sizes(sizes),
    _strides(strides),
    _offsets(offsets),
    _uniforms(uniforms)
{
}


ShaderStruct::ShaderStruct( const ShaderStruct& copy ) :
    _source(copy._source),
    _attributes(copy._attributes),
    _sizes(copy._sizes),
    _strides(copy._strides),
    _offsets(copy._offsets),
    _uniforms(copy._uniforms)
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
    return *this;
}


ShaderStruct::ShaderStruct()
{
}



}
}


#endif