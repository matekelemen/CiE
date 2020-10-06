// --- Internal Includes ---
#include "cieutils/packages/output/inc/fileinfo.hpp"
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- STL Includes ---
#include <filesystem>


namespace cie::utils::detail {


const std::string fileExtension( const std::string& fileName )
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Find extension begin
    size_t begin = fileName.find_last_of( '.' );
    if ( begin == std::string::npos )
        begin = 0;
    else if ( fileName.size()>begin )
        ++begin;

    auto extension = fileName.substr( begin );
    return extension;

    CIE_END_EXCEPTION_TRACING
}


const std::string fileName( const std::string& fileName )
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Find last folder separator
    size_t begin = fileName.find_last_of( '/' );
    if ( begin == std::string::npos )
        begin = 0;
    else
        ++begin;

    auto name = fileName.substr( begin );
    return name;

    CIE_END_EXCEPTION_TRACING
}


const std::string fileDirectory( const std::string& fileName )
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Find last folder separator
    size_t end = fileName.find_last_of( '/' );
    if ( end == std::string::npos )
        end = 0;

    auto name = fileName.substr( 0, end );
    return name;

    CIE_END_EXCEPTION_TRACING
}


bool isFile( const std::string& fileName )
{
    CIE_BEGIN_EXCEPTION_TRACING

    return std::filesystem::is_regular_file( fileName );

    CIE_END_EXCEPTION_TRACING
}


bool isTextExtension( const std::string& fileName )
{
    CIE_BEGIN_EXCEPTION_TRACING

    bool result     = false;
    for ( auto it=textExtensions.begin(); it!=textExtensions.end(); ++it )
        if ( fileName == *it )
        {
            result = true;
            break;
        }
    return result;

    CIE_END_EXCEPTION_TRACING
}


bool isBinaryExtension( const std::string& fileName )
{
    CIE_BEGIN_EXCEPTION_TRACING

    bool result     = false;
    for ( auto it=binaryExtensions.begin(); it!=binaryExtensions.end(); ++it )
        if ( fileName == *it )
        {
            result = true;
            break;
        }
    return result;

    CIE_END_EXCEPTION_TRACING
}


bool isTextFile( const std::string& fileName )
{
    CIE_BEGIN_EXCEPTION_TRACING

    if ( !isFile(fileName) )
        return false;

    return isTextExtension( fileExtension(fileName) );

    CIE_END_EXCEPTION_TRACING
}


bool isBinaryFile( const std::string& fileName )
{
    CIE_BEGIN_EXCEPTION_TRACING

    if ( !isFile(fileName) )
        return false;

    return isBinaryExtension( fileExtension(fileName) );

    CIE_END_EXCEPTION_TRACING
}


bool isDirectory( const std::string& fileName )
{
    CIE_BEGIN_EXCEPTION_TRACING

    return std::filesystem::is_directory( fileName );

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::utils::detail