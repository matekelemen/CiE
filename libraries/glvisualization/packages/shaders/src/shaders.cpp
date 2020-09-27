#ifndef GLVISUALIZATION_SHADERS_IMPL_HPP
#define GLVISUALIZATION_SHADERS_IMPL_HPP

// --- Utility Includes ---
#include <cieutils/file.hpp>
#include <cieutils/macros.hpp>

// --- Internal Includes ---
#include "glvisualization/packages/shaders/inc/shaders.hpp"

// --- STL Includes ---
#include <string>
#include <sstream>


namespace cie::gl {


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


ShaderStruct::ShaderStruct( const std::string& fileName )
{
    utils::FileManager manager( SHADER_PATH );
    auto& file = manager.open( fileName );

    // Parsing rules
    const std::string nameBegin     = "// "; 
    const std::string valueBegin    = ": ";

    // Parse header
    std::string line, name, value, values;
    size_t nameIndex, valueIndex;
    std::vector<std::string>* textContainer = nullptr;
    std::vector<GLuint>* numericContainer = nullptr;
    while( !file.eof() )
    {
        // Read new line
        std::getline( file, line );
        nameIndex = line.find( nameBegin );

        // Check termination
        if ( nameIndex == std::string::npos )
            break;
        else
            nameIndex += nameBegin.size();
        
        // Parse
        valueIndex  = line.find( valueBegin );
        name        = line.substr( nameIndex, valueIndex-nameIndex );
        values      = line.substr( valueIndex + valueBegin.size() );

        if ( name == "attributes" )
        {
            textContainer       = &_attributes;
            numericContainer    = nullptr;
        }
        else if ( name == "sizes" )
        {
            textContainer       = nullptr;
            numericContainer    = &_sizes;
        }
        else if ( name == "strides" )
        {
            textContainer       = nullptr;
            numericContainer    = &_strides;
        }
        else if ( name == "offsets" )
        {
            textContainer       = nullptr;
            numericContainer    = &_offsets;
        }
        else if ( name == "uniforms" )
        {
            textContainer       = &_uniforms;
            numericContainer    = nullptr;
        }
        else if ( name == "textures" )
        {
            textContainer       = &_textures;
            numericContainer    = nullptr;
        }
        else
            CIE_THROW( std::runtime_error, "Unrecognized shader property: " + name )

        // Convert to stream and parse
        std::istringstream stringStream(values);
        while( std::getline(stringStream, value, ',') )
        {
            if ( !value.empty() )
            {
                if (textContainer != nullptr)
                    textContainer->push_back( value );
                else if (numericContainer != nullptr)
                    numericContainer->push_back( std::atoi(value.c_str()) );
                else
                    CIE_THROW( NullPtrException, "Unset container pointer!" )
            }

        }
        
    }

    // Get shader source
    std::stringstream stream;
    stream << file.rdbuf();
    _source =  stream.str();
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


void ShaderStruct::print( std::ostream& stream ) const
{
    stream << "Attributes: ";
    for ( auto& item : _attributes )
        stream << item << ", ";
    stream << "\n";

    stream << "Sizes: ";
    for ( auto& item : _sizes )
        stream << item << ", ";
    stream << "\n";

    stream << "Strides: ";
    for ( auto& item : _strides )
        stream << item << ", ";
    stream << "\n";

    stream << "Offsets: ";
    for ( auto& item : _offsets )
        stream << item << ", ";
    stream << "\n";

    stream << "Uniforms: ";
    for ( auto& item : _uniforms )
        stream << item << ", ";
    stream << "\n";

    stream << "Textures: ";
    for ( auto& item : _textures )
        stream << item << ", ";
    stream << "\n";

    stream << "Code:\n";
    stream << _source << "\n";

    std::flush(stream);
}



} // namespace cie::gl


#endif