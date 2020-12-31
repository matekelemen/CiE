// --- External Includes ---
#include <glm/glm.hpp>

// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/macros/inc/checks.hpp"

// --- Internal Includes ---
#include "ciegl/packages/file/inc/Triangulated3DPart.hpp"

// --- STL Includes ---
#include <fstream>


namespace cie::gl {


void Triangulated3DPart::writeSTL( const std::filesystem::path& r_path ) const
{
    CIE_BEGIN_EXCEPTION_TRACING

    CIE_CHECK( this->dimension() == 3, "writing to STL is only implemented in 3D" )

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
    uint16_t attributeByteCount = this->attributeByteCount();
    
    for ( Size triangleIndex=0; triangleIndex<numberOfTriangles; ++triangleIndex )
    {
        Size triangleBegin = triangleIndex * this->primitiveVertexSize();
        
        Size p0Begin       = this->_indices[triangleBegin];
        Size p1Begin       = this->_indices[triangleBegin + 1];
        Size p2Begin       = this->_indices[triangleBegin + 2];

        // Compute normal
        glm::vec3 p0( _data[triangleBegin], _data[triangleBegin+1], _data[triangleBegin+2] );

        glm::vec3 normal = glm::normalize(glm::cross(
            glm::vec3( _data[p2Begin], _data[p2Begin+1], _data[p2Begin+2] ) - p0,
            glm::vec3( _data[p1Begin], _data[p1Begin+1], _data[p1Begin+2] ) - p0
        ));

        // Write normal
        for ( Size dim=0; dim<this->dimension(); ++dim )
            file.write( reinterpret_cast<char*>(&normal[dim]), sizeof(Part::data_type) );

        // Write points
        for ( Size pointBegin : { p0Begin, p1Begin, p2Begin } )
            file.write( reinterpret_cast<const char*>( &_data[pointBegin] ), this->vertexAttributeSize() * sizeof( Triangulated3DPart::data_type ) );

        // Write attribute byte count
        file.write( reinterpret_cast<char*>( &attributeByteCount ), sizeof( attributeByteCount ) );
    }

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::gl