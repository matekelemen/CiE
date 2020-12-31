// --- Internal Includes ---
#include "cieutils/packages/output/inc/fileinfo.hpp"
#include "cieutils/packages/macros/inc/exceptions.hpp"

// --- STL Includes ---
#include <filesystem>


namespace cie::utils::detail {


std::filesystem::path fileExtension( const std::filesystem::path& r_filePath )
{
    CIE_BEGIN_EXCEPTION_TRACING

    return r_filePath.extension();

    CIE_END_EXCEPTION_TRACING
}


std::filesystem::path fileName(const std::filesystem::path& r_filePath )
{
    CIE_BEGIN_EXCEPTION_TRACING

    return std::filesystem::path( r_filePath ).filename();

    CIE_END_EXCEPTION_TRACING
}


std::filesystem::path fileDirectory(const std::filesystem::path& r_filePath )
{
    CIE_BEGIN_EXCEPTION_TRACING

    return r_filePath.parent_path();

    CIE_END_EXCEPTION_TRACING
}


bool isFile(const std::filesystem::path& r_filePath )
{
    CIE_BEGIN_EXCEPTION_TRACING

    return std::filesystem::is_regular_file( r_filePath );

    CIE_END_EXCEPTION_TRACING
}


bool isTextExtension(const std::filesystem::path& r_extension )
{
    CIE_BEGIN_EXCEPTION_TRACING

    for ( const auto& r_textExtension : textExtensions )
        if ( r_extension == r_textExtension )
            return true;
    return false;

    CIE_END_EXCEPTION_TRACING
}


bool isBinaryExtension(const std::filesystem::path& r_extension)
{
    CIE_BEGIN_EXCEPTION_TRACING

    for ( const auto& r_binaryExtension : binaryExtensions )
        if ( r_extension == r_binaryExtension )
            return true;
    return false;

    CIE_END_EXCEPTION_TRACING
}


bool isTextFile(const std::filesystem::path& r_filePath )
{
    CIE_BEGIN_EXCEPTION_TRACING

    if ( !isFile(r_filePath) )
        return false;

    return isTextExtension( fileExtension(r_filePath) );

    CIE_END_EXCEPTION_TRACING
}


bool isBinaryFile(const std::filesystem::path& r_filePath )
{
    CIE_BEGIN_EXCEPTION_TRACING

    if ( !isFile(r_filePath) )
        return false;

    return isBinaryExtension( fileExtension(r_filePath) );

    CIE_END_EXCEPTION_TRACING
}


bool isDirectory(const std::filesystem::path& r_filePath )
{
    CIE_BEGIN_EXCEPTION_TRACING

    return std::filesystem::is_directory( r_filePath );

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::utils::detail