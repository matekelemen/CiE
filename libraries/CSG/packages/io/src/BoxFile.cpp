// --- Utility Includes ---
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cieutils/packages/macros/inc/checks.hpp"
#include "cieutils/packages/output/inc/fileinfo.hpp"

// --- Internal Includes ---
#include "CSG/packages/io/inc/BoxFile.hpp"


namespace cie::csg {


BoxFile::BoxFile( const std::filesystem::path& r_path,
                  Size dimension,
                  Size coordinateByteSize ) :
    _extension( ".boxes" ),
    _dimension( dimension ),
    _coordinateByteSize( coordinateByteSize ),
    _fileManager( utils::detail::fileDirectory(r_path) ),
    _p_file()
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Check extension
    CIE_CHECK( r_path.extension().string() == this->_extension, "BoxFile with invalid extension: " + r_path.extension().string() )

    // File doesn't exist -> output
    if ( !std::filesystem::exists(r_path) )
    {
        // Check dimension and byte size
        CIE_CHECK( this->_dimension != 0, "Uninitialized dimension in output BoxFile" )
        CIE_CHECK( this->_coordinateByteSize != 0, "Uninitialized coordinate byte size in output BoxFile" )

        // Open file
        this->_p_file = _fileManager.filePtr( _fileManager.newFile( utils::detail::fileName(r_path) ) );

        // Write header
        uint64_t dim      = dimension;
        uint64_t byteSize = coordinateByteSize;

        this->_p_file->write( reinterpret_cast<char*>(&dim), sizeof(dim) );
        this->_p_file->write( reinterpret_cast<char*>(&byteSize), sizeof(byteSize) );
    }

    // File exists -> input
    else
    {
        // Open file
        this->_p_file = this->_fileManager.filePtr( this->_fileManager.open( r_path ) );

        // Read header
        uint64_t dim      = 0;
        uint64_t byteSize = 0;

        this->_p_file->read( reinterpret_cast<char*>(&dim), sizeof(dim) );
        this->_p_file->read( reinterpret_cast<char*>(&byteSize), sizeof(byteSize) );

        this->_dimension          = dim;
        this->_coordinateByteSize = byteSize;

        CIE_CHECK( this->_dimension != 0, "Uninitialized dimension in input BoxFile" )
        CIE_CHECK( this->_coordinateByteSize != 0, "Uninitialized coordinate byte size in input BoxFile" )
    }

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::csg