// --- External Includes ---
#include <glm/glm.hpp>

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/checks.hpp"

// --- Internal Includes ---
#include "ciegl/packages/file/inc/Part.hpp"

// --- STL Includes ---
#include <fstream>


namespace cie::gl {


Part::Part( Size dimension,
            Size primitiveByteSize ) :
    _data(),
    _dimension( dimension ),
    _primitiveByteSize( primitiveByteSize )
{
    CIE_CHECK(
        this->_dimension > 0,
        "Number of dimensions must be positive, got " + std::to_string(this->_dimension) )

    CIE_CHECK(
        this->_primitiveByteSize > 0,
        "Primitive byte size must be positive, got" + std::to_string(this->_primitiveByteSize) )
}


Part::Part( Size dimension,
            Size primitiveByteSize,
            std::istream& r_stream ) :
    Part( dimension, primitiveByteSize )
{
    this->load( r_stream );
}


Size Part::byteCount() const
{
    return this->_data.size() * sizeof( Part::data_type );
}


Size Part::numberOfPrimitives() const
{
    return this->byteCount() / this->_primitiveByteSize;
}


void Part::load( std::istream& r_stream )
{
    CIE_THROW( AbstractCallException, "Call to pure virtual Part::load" )
}


const Part::data_container& Part::data() const
{
    return this->_data;
}


void Part::writeSTL( const std::filesystem::path& r_path ) const
{
    CIE_BEGIN_EXCEPTION_TRACING

    CIE_CHECK( this->_dimension == 3, "writing to STL is only implemented in 3D" )

    // Create file
    std::ofstream file;
    file.open( r_path, std::ios::binary );

    if ( !file.is_open() )
        CIE_THROW( Exception, "Could not create file: " + r_path.string() )

    // Header
    char header = ' ';
    for ( Size i=0; i<80; ++i )
        file.write( reinterpret_cast<char*>(&header), sizeof(header) );

    // Number of triangles
    uint32_t numberOfTriangles = this->numberOfPrimitives();
    file.write( reinterpret_cast<char*>( &numberOfTriangles ), sizeof(numberOfTriangles) );

    // Attributes
    uint16_t attributeByteCount = (1+3) * this->_dimension * sizeof( Part::data_type );
    
    for ( Size triangleIndex=0; triangleIndex<numberOfTriangles; ++triangleIndex )
    {
        Size triangleBegin = triangleIndex * 3 * this->_dimension;
        Size p1Begin       = triangleBegin + this->_dimension;
        Size p2Begin       = p1Begin + this->_dimension;
        Size triangleEnd   = p2Begin + this->_dimension;

        // Compute normal
        glm::vec3 p0( _data[triangleBegin], _data[triangleBegin+1], _data[triangleBegin+2] );

        glm::vec3 normal = glm::normalize(glm::cross(
            glm::vec3( _data[p2Begin], _data[p2Begin+1], _data[p2Begin+2] ) - p0,
            glm::vec3( _data[p1Begin], _data[p1Begin+1], _data[p1Begin+2] ) - p0
        ));

        // Write normal
        for ( Size dim=0; dim<this->_dimension; ++dim )
            file.write( reinterpret_cast<char*>(&normal[dim]), sizeof(Part::data_type) );

        // Write points
        for ( Size attributeIndex=triangleBegin; attributeIndex<triangleEnd; ++attributeIndex )
            file.write( reinterpret_cast<const char*>( &_data[attributeIndex] ), sizeof( Part::data_type ) );

        // Write attribute byte count
        file.write( reinterpret_cast<char*>( &attributeByteCount ), sizeof( attributeByteCount ) );
    }

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::gl


