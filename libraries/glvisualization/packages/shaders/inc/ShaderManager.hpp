#ifndef GLVISUALIZATION_SHADER_ASSEMBLER_HPP
#define GLVISUALIZATION_SHADER_ASSEMBLER_HPP

// --- External Includes ---
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// --- Internal Includes ---
#include "shaders.hpp"
#include "../../context/inc/AbsContextClass.hpp"

// --- STL Includes ---
#include <string>
#include <vector>

namespace cie {
namespace gl {

using Shader    = char;
using ShaderPtr = Shader*;


class ShaderManager : public AbsContextClass
{
public:
    ShaderManager( GLContext& context );
    ~ShaderManager();
    ShaderPtr getVertexShader();
    ShaderPtr getGeometryShader();
    ShaderPtr getFragmentShader();

    void setVertexShader( const ShaderStruct& shader );
    void setGeometryShader( const ShaderStruct& shader );
    void setFragmentShader( const ShaderStruct& shader );

    void setVertexShader( const std::string& fileName );
    void setGeometryShader( const std::string& fileName );
    void setFragmentShader( const std::string& fileName );

    const std::vector<std::string>& attributes() const;
    const std::vector<GLuint>& attributeSizes() const;
    const std::vector<GLuint>& attributeStrides() const;
    const std::vector<GLuint>& attributeOffsets() const;
    const std::vector<std::string>& fragOutputNames() const;
    const std::vector<std::string>& uniforms() const;

    const ShaderStruct vertexShader() const;
    const ShaderStruct geometryShader() const;
    const ShaderStruct fragmentShader() const;

protected:
    ShaderStruct    _vertexShader;
    ShaderStruct    _geometryShader;
    ShaderStruct    _fragmentShader;

private:
    ShaderPtr   _vertexPtr;
    ShaderPtr   _geometryPtr;
    ShaderPtr   _fragmentPtr;
};


}
}

#endif