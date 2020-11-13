// --- External Includes ---
#include <glm/glm.hpp>

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- Internal Includes ---
#include "ciegl/packages/file/inc/STLPart.hpp"

// --- STL Includes ---
#include <cstdint>
#include <array>
#include <algorithm>


namespace cie::gl {


STLPart::STLPart( std::istream& r_stream )
{
    this->load( r_stream );
}


STLPart::STLPart( const std::string& r_fileName )
{
    CIE_BEGIN_EXCEPTION_TRACING

    std::ifstream file;
    file.open( r_fileName,
               std::ios::binary );

    if ( !file )
        CIE_THROW( Exception, "Failed to open file: " + r_fileName )

    this->load( file );

    CIE_END_EXCEPTION_TRACING
}


Size STLPart::byteCount() const
{
    return this->_data.size() * sizeof( typename STLPart::data_type );
}


Size STLPart::primitiveCount() const
{
    return this->byteCount() / STLPart::primitive_byte_size;
}


void STLPart::load( std::istream& r_stream )
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Header - 80 chars
    {
        char tmp;

        for ( Size i=0; i<80; ++i )
        {
            r_stream.read( reinterpret_cast<char*>(&tmp), sizeof(tmp) );
        }
    }

    // Number of triangles
    Size numberOfTriangles;

    {
        uint32_t tmp;
        r_stream.read( reinterpret_cast<char*>(&tmp), sizeof(tmp) );

        numberOfTriangles = Size(tmp);
    }

    // Allocate memory for the data
    this->_data.reserve( numberOfTriangles * STLPart::primitive_byte_size / sizeof(typename STLPart::data_type) );

    // Triangles and their normals
    using data_type = typename STLPart::data_type;
    data_type tmp;
    uint16_t attributeByteCount;
    std::array<data_type,STLPart::dimension> normal;

    for ( Size triangleIndex=0; triangleIndex<numberOfTriangles; ++triangleIndex )
    {
        // Read normal
        for ( Size componentIndex=0; componentIndex<STLPart::dimension; ++componentIndex )
        {
            r_stream.read( reinterpret_cast<char*>(&tmp), sizeof(tmp) );
            normal[componentIndex] = tmp;
        }

        // Read points
        for ( Size pointIndex=0; pointIndex<3; pointIndex++ )
        {
            for ( Size componentIndex=0; componentIndex<STLPart::dimension; ++componentIndex )
            {
                r_stream.read( reinterpret_cast<char*>(&tmp), sizeof(tmp) );
                this->_data.push_back( tmp );
            }
            for ( Size componentIndex=0; componentIndex<STLPart::dimension; ++componentIndex )
                this->_data.push_back( normal[componentIndex] );
        }

        // Read "attribute byte count"
        r_stream.read( reinterpret_cast<char*>(&attributeByteCount), sizeof(attributeByteCount) );
    }

    if ( r_stream.peek() != EOF )
        CIE_THROW( Exception, "Reading STL file failed" )

    CIE_END_EXCEPTION_TRACING
}


void STLPart::repairOrientation()
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Total number of attributes for a single vertex/triangle
    const Size vertexAttributeCount   = 2 * STLPart::dimension;
    const Size triangleAttributeCount = 3 * vertexAttributeCount;
    Size baseIndex;

    for ( Size attributeIndex=0; attributeIndex<this->_data.size(); attributeIndex+=triangleAttributeCount )
    {
        glm::vec3 base(
            _data[attributeIndex],
            _data[attributeIndex+1],
            _data[attributeIndex+2]
        );

        glm::vec3 normal(
            _data[attributeIndex+3],
            _data[attributeIndex+4],
            _data[attributeIndex+5]
        );

        baseIndex = attributeIndex + 2 * vertexAttributeCount;

        glm::vec3 side1 = glm::vec3(
            _data[baseIndex],
            _data[baseIndex+1],
            _data[baseIndex+2]
            ) 
            - base;

        baseIndex = attributeIndex + vertexAttributeCount;

        glm::vec3 side2 = glm::vec3(
            _data[baseIndex],
            _data[baseIndex+1],
            _data[baseIndex+2]
            )
            - base;

        if ( glm::dot( normal, glm::cross(side1,side2) ) < 0 ) // flipped normals
        {
            baseIndex = attributeIndex + vertexAttributeCount;

            for ( Size componentIndex=0; componentIndex<STLPart::dimension; ++componentIndex )
                std::swap(
                    _data[attributeIndex+componentIndex],
                    _data[baseIndex + componentIndex]
                );
        }
    }

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::gl