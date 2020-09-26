// --- Internal Includes ---
#include "../inc/fileinfo.hpp"

// --- STL Includes ---
#include <filesystem>


namespace cie::utils::detail {


const std::string fileExtension( const std::string& fileName )
{
    // Find extension begin
    size_t begin = fileName.find_last_of( '.' );
    if ( begin == std::string::npos )
        begin = 0;
    else if ( fileName.size()>begin )
        ++begin;

    auto extension = fileName.substr( begin );
    return extension;
}


const std::string fileName( const std::string& fileName )
{
    // Find last folder separator
    size_t begin = fileName.find_last_of( '/' );
    if ( begin == std::string::npos )
        begin = 0;
    else
        ++begin;

    auto name = fileName.substr( begin );
    return name;
}


const std::string fileDirectory( const std::string& fileName )
{
    // Find last folder separator
    size_t end = fileName.find_last_of( '/' );
    if ( end == std::string::npos )
        end = 0;

    auto name = fileName.substr( 0, end );
    return name;
}


bool isFile( const std::string& fileName )
{
    return std::filesystem::is_regular_file( fileName );
}


bool isTextExtension( const std::string& fileName )
{
    bool result     = false;
    for ( auto it=textExtensions.begin(); it!=textExtensions.end(); ++it )
        if ( fileName == *it )
        {
            result = true;
            break;
        }
    return result;
}


bool isBinaryExtension( const std::string& fileName )
{
    bool result     = false;
    for ( auto it=binaryExtensions.begin(); it!=binaryExtensions.end(); ++it )
        if ( fileName == *it )
        {
            result = true;
            break;
        }
    return result;
}


bool isTextFile( const std::string& fileName )
{
    if ( !isFile(fileName) )
        return false;

    return isTextExtension( fileExtension(fileName) );
}


bool isBinaryFile( const std::string& fileName )
{
    if ( !isFile(fileName) )
        return false;

    return isBinaryExtension( fileExtension(fileName) );
}


bool isDirectory( const std::string& fileName )
{
    return std::filesystem::is_directory( fileName );
}


} // namespace cie::utils::detail