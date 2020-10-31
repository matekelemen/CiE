// --- External Includes ---
#include "pugixml.hpp"

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- Internal Includes ---
#include "ciegl/packages/shaders/inc/Shader.hpp"

// --- STL Includes ---
#include <fstream>
#include <sstream>
#include <limits>


namespace cie::gl {


Shader::Shader( const std::string& r_configPath,
                const std::string& r_codePath ) :
    utils::IDObject<Size>( std::numeric_limits<Size>().max() )
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Check input files
    pugi::xml_document configuration;
    pugi::xml_parse_result configParseResult = configuration.load_file( r_configPath.c_str() );
    std::ifstream codeFile( r_codePath );

    if ( !configParseResult )
        CIE_THROW( 
            Exception,
            "Error loading shader configuration file: " + r_configPath + "\n" + configParseResult.description()
            )

    if ( !codeFile.is_open() )
        CIE_THROW( Exception, "Unable to open shader code file: " + r_codePath )

    // Get shader root
    pugi::xml_node root = configuration.child( "shader" );

    if ( !root )
        CIE_THROW( Exception, "Unable to find shader configuration root" )

    // Lambda for getting and validating nodes
    pugi::xml_node configNode;
    auto getChildNode = [&root]( const std::string& r_name ) -> pugi::xml_node
    {
        auto child = root.child( r_name.c_str() );
        if ( !child )
            CIE_THROW( Exception, r_name + " not found in shader configuration" )

        return child;
    };

    // Parse config file - attributes
    configNode = getChildNode( "attributes" );

    for ( auto attribute : configNode )
    {
        _attributes.emplace_back(
            attribute.attribute("name").value(),
            attribute.attribute("size").as_int(),
            attribute.attribute("stride").as_int(),
            attribute.attribute("offset").as_int()
        );
    }

    // Parse config file - uniforms
    configNode = getChildNode( "uniforms" );
    for ( auto uniform : configNode )
    {
        _uniforms.emplace_back(
            uniform.attribute("name").value(),
            uniform.attribute("size").as_int(),
            uniform.attribute("type").value()
        );
    }

    // Parse config file - textures
    configNode = getChildNode( "textures" );
    for ( auto texture : configNode )
    {
        _textures.emplace_back(
            texture.attribute("name").value(),
            texture.attribute("size").as_int(),
            texture.attribute("type").value()
        );
    }

    // Parse config file - outputs
    configNode = getChildNode( "outputs" );
    Size counter = 0;
    for ( auto output : configNode )
    {
        _outputs.emplace_back(
            output.attribute( "name" ).value(),
            counter++
        );
    }

    // Load shader code
    std::stringstream stream;
    stream << codeFile.rdbuf();
    _source = stream.str();

    CIE_END_EXCEPTION_TRACING
}


void Shader::print( std::ostream& r_stream ) const
{
    CIE_BEGIN_EXCEPTION_TRACING

    r_stream << this->_source << std::endl;

    CIE_END_EXCEPTION_TRACING
}


const std::string& Shader::source() const
{
    return _source;
}


const typename Shader::attribute_container& Shader::attributes() const
{
    return _attributes;
}


const typename Shader::uniform_container& Shader::uniforms() const
{
    return _uniforms;
}


const typename Shader::texture_container& Shader::textures() const
{
    return _textures;
}


const typename Shader::output_container& Shader::outputs() const
{
    return _outputs;
}


} // namespace cie::gl