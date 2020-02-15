#include "../inc/ShaderAssembler.hpp"
#include "../inc/shaders.hpp"
#include <algorithm>
#include <iostream>

namespace cie {
namespace gl {


ShaderAssembler::ShaderAssembler() :
    _vertexShader( defaultColorVertexShader ),
    _geometryShader( defaultColorGeometryShader ),
    _fragmentShader( defaultColorFragmentShader ),
    _vertexPtr(nullptr),
    _geometryPtr(nullptr),
    _fragmentPtr(nullptr)
{
}


ShaderAssembler::~ShaderAssembler()
{
    if (_vertexPtr != nullptr)
        delete _vertexPtr;
    if (_geometryPtr != nullptr)
        delete _geometryPtr;
    if (_fragmentPtr != nullptr)
        delete _fragmentPtr;
}


ShaderPtr ShaderAssembler::getVertexShader()
{
    if (_vertexPtr != nullptr)
        delete _vertexPtr;

    _vertexPtr  = new Shader[_vertexShader._source.size()];
    std::copy( _vertexShader._source.begin(), _vertexShader._source.end(), _vertexPtr );
    _vertexPtr[ _vertexShader._source.size()-1 ] = '\0';

    return _vertexPtr;
}


ShaderPtr ShaderAssembler::getGeometryShader()
{
    if (_geometryPtr != nullptr)
        delete _geometryPtr;

    _geometryPtr  = new Shader[_geometryShader._source.size()];
    std::copy( _geometryShader._source.begin(), _geometryShader._source.end(), _geometryPtr );
    _geometryPtr[ _geometryShader._source.size()-1 ] = '\0';
    
    return _geometryPtr;
}


ShaderPtr ShaderAssembler::getFragmentShader()
{
    if (_fragmentPtr != nullptr)
        delete _fragmentPtr;

    _fragmentPtr  = new Shader[_fragmentShader._source.size()];
    std::copy( _fragmentShader._source.begin(), _fragmentShader._source.end(), _fragmentPtr );
    _fragmentPtr[ _fragmentShader._source.size()-1 ] = '\0';
    
    return _fragmentPtr;
}


const std::vector<std::string>&  ShaderAssembler::attributes() const
{
    return _vertexShader._attributes;
}


const std::vector<GLuint>& ShaderAssembler::attributeSizes() const
{
    return _vertexShader._sizes;
}


const std::vector<GLuint>& ShaderAssembler::attributeStrides() const
{
    return _vertexShader._strides;
}


const std::vector<GLuint>& ShaderAssembler::attributeOffsets() const
{
    return _vertexShader._offsets;
}


const std::vector<std::string>& ShaderAssembler::fragOutputNames() const
{
    return _fragmentShader._attributes;
}




}
}