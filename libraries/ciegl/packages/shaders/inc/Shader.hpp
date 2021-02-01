#ifndef CIE_GL_SHADER_HPP
#define CIE_GL_SHADER_HPP

// --- Utility Includes ---
#include "cieutils/packages/types/inc/IDObject.hpp"
#include "cieutils/packages/types/inc/NamedObject.hpp"
#include "cieutils/packages/types/inc/types.hpp"

// --- STL Includes ---
#include <string>
#include <deque>
#include <iostream>
#include <memory>
#include <filesystem>


/** --- XML SHADER CONFIGURATION LAYOUT ---
 * 
 * <shader>
 *      <attributes>
 *          <a0 name="" size="" stride="" offset="">
 *          .
 *          .
 *          .
 *      </attributes>
 *      <uniforms>
 *          <u0 name="">
 *          .
 *          .
 *          .
 *      </uniforms>
 *      <textures>
 *          <t0 name="">
 *          .
 *          .
 *          .
 *      </textures>
 *      <outputs>
 *      .
 *      .
 *      .
 *      </outputs>
 * </shader>
 * 
 * ----------------------------------------
 */


namespace cie::gl {


class Shader;
using ShaderPtr      = std::shared_ptr<Shader>;
using ShaderConstPtr = std::shared_ptr<const Shader>;


class Shader : public utils::IDObject<Size>
{
public:
    /* --- MEMBER CLASSES --- */

    /**
     * Represents an attribute passed to each vertex.
     * For example: position, texture coordinates, etc.
     * Requires:
     *  - name      : name of the variable in the shader (eg: position)
     *  - size      : number of components that make up the attribute (eg: x,y,z -> 3)
     *  - stride    : number of bytes in between the components of the attribute
     *                (that are not part of the attribute itself)
     *  - offset    : number of bytes before the first component of the attribute
     */
    class Attribute : public utils::NamedObject
    {
    public:
        Attribute( const std::string& r_name,
                   Size size,
                   Size stride,
                   Size offset ) :
            utils::NamedObject( r_name ),
            _size( size ),
            _stride( stride ),
            _offset( offset )
        {}

        const Size size() const     { return _size; }
        const Size stride() const   { return _stride; }
        const Size offset() const   { return _offset; }
        
    private:
        const Size _size;
        const Size _stride;
        const Size _offset;
    };


    /**
     * Data global to all vertices.
     * For example: transformation matrix, background color, etc.
     * Requires:
     *  - name      : name of the uniform in the shader code
     *  - size      : number of components that make up the uniform
     *  - type      : data type
     */
    class Uniform : public utils::NamedObject
    {
    public:
        Uniform( const std::string& r_name,
                 Size size,
                 const std::string& r_type ) :
            utils::NamedObject( r_name ),
            _size( size ),
            _type( r_type )
        {}

        Uniform( const Uniform& r_rhs ) = default;

        const Size size() const         { return _size; }
        const std::string& type() const { return _type; }

    protected:
        const Size        _size;
        const std::string _type;
    };


    /// Represents a texture in a shader
    class Texture : public Uniform
    {
    public:
        /**
         * @param r_name name of the texture in the shader
         * @param size number of dimensions (1, 2 or 3)
         * @param channels number of components in a pixel (example: RGB -> 3)
         * @param r_type texture data type ()
         */
        Texture( const std::string& r_name,
                 Size size,
                 Size channels,
                 const std::string& r_type ) :
            Uniform( r_name, size, r_type ),
            _channels( channels )
        {}

    protected:
        Size _channels;
    };

    /// Represents an output of the fragment shader
    class Output :
        public utils::NamedObject,
        public utils::IDObject<Size>
    {
    public:
        Output( const std::string& r_name, Size id ) :
            utils::NamedObject( r_name ),
            utils::IDObject<Size>( id )
        {}
    };

public:
    /* --- TYPE ALIASES --- */

    using attribute_container   = std::deque<Attribute>;
    using uniform_container     = std::deque<Uniform>;
    using texture_container     = std::deque<Texture>;
    using output_container      = std::deque<Output>;

public:
    /* --- MEMBER FUNCTIONS --- */

    void print( std::ostream& r_stream = std::cout ) const;

    const std::string& source() const;
    const attribute_container& attributes() const;
    const uniform_container& uniforms() const;
    const texture_container& textures() const;
    const output_container& outputs() const;

protected:
    Shader( const std::filesystem::path& r_configPath,
            const std::filesystem::path& r_codePath );

public:
    /* --- RELATED FREE FLOATS --- */

    /**
     * Create a vertex shader from a GLSL source file and its
     * configuration .xml file. The configuration is checked for
     * the basic requirements for a vertex shader.
     * 
     * Note that consistency between the source and configuration
     * is not checked.
     */
    template <class ShaderType, class ...Args>
    friend ShaderPtr makeVertexShader( const std::filesystem::path& r_configPath,
                                       const std::filesystem::path& r_codePath,
                                       Args&&... args );


    /**
     * Create a geometry shader from a GLSL source file and its
     * configuration .xml file. The configuration is checked for
     * the basic requirements for a geometry shader.
     * 
     * Note that consistency between the source and configuration
     * is not checked.
     */
    template <class ShaderType, class ...Args>
    friend ShaderPtr makeGeometryShader( const std::filesystem::path& r_configPath,
                                         const std::filesystem::path& r_codePath,
                                         Args&&... args );


    /**
     * Create a fragment shader from a GLSL source file and its
     * configuration .xml file. The configuration is checked for
     * the basic requirements for a fragment shader.
     * 
     * Note that consistency between the source and configuration
     * is not checked.
     */
    template <class ShaderType, class ...Args>
    friend ShaderPtr makeFragmentShader( const std::filesystem::path& r_configPath,
                                         const std::filesystem::path& r_codePath,
                                         Args&&... args );

protected:
    /* --- MEMBER VARIABLES --- */

    std::string         _source;
    attribute_container _attributes;
    uniform_container   _uniforms;
    texture_container   _textures;
    output_container    _outputs;
};


} // namespace cie::gl

#include "ciegl/packages/shaders/impl/Shader_impl.hpp"

#endif