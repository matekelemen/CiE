// --- Internal Includes ---
#include "ciegl/packages/shaders/inc/ShaderManager.hpp"
#include "ciegl/packages/shaders/inc/shaders.hpp"

// --- STL Includes ---
#include <algorithm>
#include <iostream>

namespace cie::gl {


ShaderManager::ShaderManager( GLFWContext& context ) :
    AbsContextClass( context, "ShaderManager" ),
    _vertexShader( "defaultColorVertexShader.glsl" ),
    _geometryShader( "defaultColorGeometryShader.glsl" ),
    _fragmentShader( "defaultColorFragmentShader.glsl" ),
    _vertexPtr(nullptr),
    _geometryPtr(nullptr),
    _fragmentPtr(nullptr)
{
}


ShaderManager::~ShaderManager()
{
    if (_vertexPtr != nullptr)
        delete[] _vertexPtr;
    if (_geometryPtr != nullptr)
        delete[] _geometryPtr;
    if (_fragmentPtr != nullptr)
        delete[] _fragmentPtr;
}


ShaderPtr ShaderManager::getVertexShader()
{
    if (_vertexPtr != nullptr)
        delete _vertexPtr;

    _vertexPtr  = new Shader[_vertexShader._source.size()+1];
    std::copy( _vertexShader._source.begin(), _vertexShader._source.end(), _vertexPtr );
    _vertexPtr[ _vertexShader._source.size() ] = '\0';

    return _vertexPtr;
}


ShaderPtr ShaderManager::getGeometryShader()
{
    if (_geometryPtr != nullptr)
        delete _geometryPtr;

    _geometryPtr  = new Shader[_geometryShader._source.size()+1];
    std::copy( _geometryShader._source.begin(), _geometryShader._source.end(), _geometryPtr );
    _geometryPtr[ _geometryShader._source.size() ] = '\0';
    
    return _geometryPtr;
}


ShaderPtr ShaderManager::getFragmentShader()
{
    if (_fragmentPtr != nullptr)
        delete _fragmentPtr;

    _fragmentPtr  = new Shader[_fragmentShader._source.size()+1];
    std::copy( _fragmentShader._source.begin(), _fragmentShader._source.end(), _fragmentPtr );
    _fragmentPtr[ _fragmentShader._source.size() ] = '\0';
    
    return _fragmentPtr;
}


void ShaderManager::setVertexShader( const ShaderStruct& shader )
{
    _vertexShader = shader;
    log( "Set new vertex shader" );
}


void ShaderManager::setGeometryShader( const ShaderStruct& shader )
{
    _geometryShader = shader;
    log( "Set new geometry shader" );
}


void ShaderManager::setFragmentShader( const ShaderStruct& shader )
{
    _fragmentShader = shader;
    log( "Set new fragment shader" );
}


void ShaderManager::setVertexShader( const std::string& fileName )
{
    _vertexShader = ShaderStruct( fileName );
    log( "Set new vertex shader: " + fileName );
}


void ShaderManager::setGeometryShader( const std::string& fileName )
{
    _geometryShader = ShaderStruct( fileName );
    log( "Set new geometry shader: " + fileName );
}


void ShaderManager::setFragmentShader( const std::string& fileName )
{
    _fragmentShader = ShaderStruct( fileName );
    log( "Set new fragment shader: " + fileName );
}


const std::vector<std::string>&  ShaderManager::attributes() const
{
    return _vertexShader._attributes;
}


const std::vector<GLuint>& ShaderManager::attributeSizes() const
{
    return _vertexShader._sizes;
}


const std::vector<GLuint>& ShaderManager::attributeStrides() const
{
    return _vertexShader._strides;
}


const std::vector<GLuint>& ShaderManager::attributeOffsets() const
{
    return _vertexShader._offsets;
}


const std::vector<std::string>& ShaderManager::fragOutputNames() const
{
    return _fragmentShader._attributes;
}


const std::vector<std::string>& ShaderManager::uniforms() const
{
    return _vertexShader._uniforms;
}


const ShaderStruct ShaderManager::vertexShader() const
{
    return _vertexShader;
}


const ShaderStruct ShaderManager::geometryShader() const
{
    return _geometryShader;
}


const ShaderStruct ShaderManager::fragmentShader() const
{
    return _fragmentShader;
}


} // namespace cie::gl