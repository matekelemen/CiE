// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- Internal Includes ---
#include "ciegl/packages/file/inc/STLPart.hpp"

// --- STL Includes ---
#include <iostream>
#include <cstdint>


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


void STLPart::load( std::istream& r_stream )
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Header - 80 chars
    {
        char tmp;

        for ( Size i=0; i<80; ++i )
        {
            r_stream.read( reinterpret_cast<char*>(&tmp), sizeof(tmp) );
            std::cout << tmp;
        }
    }

    // Number of triangles
    Size numberOfTriangles;

    {
        uint32_t tmp;
        r_stream.read( reinterpret_cast<char*>(&tmp), sizeof(tmp) );

        numberOfTriangles = Size(tmp);
        std::cout << std::endl << tmp << std::endl;
    }

    // Allocate memory for the data
    this->_data.reserve( numberOfTriangles * STLPart::primitive_byte_size );

    // Triangles and their normals
    using data_type = typename STLPart::data_type;
    data_type tmp;

    for ( Size triangleIndex=0; triangleIndex<numberOfTriangles; ++triangleIndex )
    {
        // Read normal
        for ( Size componentIndex=0; componentIndex<STLPart::dimension; ++componentIndex )
        {
            r_stream.read( reinterpret_cast<char*>(&tmp), sizeof(tmp) );
            this->_data.push_back( tmp );
        }

        // Read points
        for ( Size pointIndex=0; pointIndex<3; pointIndex++ )
            for ( Size componentIndex=0; componentIndex<STLPart::dimension; ++componentIndex )
            {
                r_stream.read( reinterpret_cast<char*>(&tmp), sizeof(tmp) );
                this->_data.push_back( tmp );
            }
    }

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::gl