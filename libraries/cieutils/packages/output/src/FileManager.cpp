// --- Internal Includes ---
#include "cieutils/packages/output/inc/FileManager.hpp"
#include "cieutils/packages/output/inc/fileinfo.hpp"
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cmake_variables.hpp"

// --- STL Includes ---
#include <filesystem>
#include <algorithm>


namespace cie::utils {


std::vector<std::filesystem::path> FileManager::_paths = {};


FileManager::FileManager( const std::filesystem::path& r_path ) :
    _path( r_path )
{
    CIE_BEGIN_EXCEPTION_TRACING

    if ( _path.empty() )
        _path = INSTALL_PATH / "output";

    if ( !detail::isDirectory( _path ) )
    {
        try
        {
            this->createPath( _path );
        }
        catch ( const std::exception& r_exception )
        {
            CIE_THROW( Exception, r_exception.what() )
        }
    }

    auto it = std::find( _paths.begin(), _paths.end(), _path );
    if (it == _paths.end() )
        _paths.push_back(_path);

    CIE_END_EXCEPTION_TRACING
}


FileManager::FileManager() :
    FileManager( INSTALL_PATH / "output" )
{ 
}


FileManager::~FileManager()
{
    closeAll();
    auto it = std::find( _paths.begin(), _paths.end(), _path );
    if (it != _paths.end() )
        _paths.erase(it);
}


bool FileManager::createPath( const std::filesystem::path& r_path )
{
    CIE_BEGIN_EXCEPTION_TRACING

    if ( !std::filesystem::exists( r_path ) )
    {
        std::filesystem::create_directories( r_path );
        return true;
    }
    else
        return false;

    CIE_END_EXCEPTION_TRACING
}


File& FileManager::newFile( const std::filesystem::path& r_filePath )
{
    CIE_BEGIN_EXCEPTION_TRACING

    std::ios_base::openmode mode = std::ios::out;

    // Determine modifier bits
    if ( detail::isBinaryExtension( detail::fileExtension(r_filePath) ) )
        mode = mode | std::ios::binary;

    // Register file stream
    return this->newFile( r_filePath, mode );

    CIE_END_EXCEPTION_TRACING
}


File& FileManager::newFile( const std::filesystem::path& r_filePath,
                            std::ios_base::openmode openMode )
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Get file path
    auto filePath = this->filePath( r_filePath );

    // Create path if it doesn't exist yet
    auto path = detail::fileDirectory( filePath );
    this->createPath( path );

    // Register file stream
    _files.push_back( std::make_shared<std::fstream>( filePath, openMode ) );
    return *_files.back();

    CIE_END_EXCEPTION_TRACING
}


File& FileManager::open( const std::filesystem::path& r_filePath )
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Get file path
    auto path = this->filePath( r_filePath );

    // Check whether file exists
    if ( !detail::isFile(path) )
        CIE_THROW( Exception, "Path not found: " + path.string() )

    // Determine modifier bits
    std::ios_base::openmode mode = std::ios::in;

    if ( detail::isBinaryExtension( detail::fileExtension( r_filePath ) ) )
        mode = mode | std::ios::binary;

    // Register file stream
    _files.push_back( std::make_shared<std::fstream>( path, mode ) );
    return *_files.back();

    CIE_END_EXCEPTION_TRACING
}


FilePtr FileManager::filePtr( const File& file )
{
    CIE_BEGIN_EXCEPTION_TRACING

    int index = -1;
    for ( auto it=_files.begin(); it!=_files.end(); ++it )
        if (it->get() == &file)
        {
            index = int(std::distance( _files.begin(), it ));
            break;
        }

    if ( index == -1 || index >= int(_files.size()) )
        CIE_THROW( Exception, "File stream not found in FileManager")

    return _files[index];

    CIE_END_EXCEPTION_TRACING
}


void FileManager::closeAll()
{
    CIE_BEGIN_EXCEPTION_TRACING

    for ( auto& rp_file : this->_files )
    {
        rp_file->close();
    }

    CIE_END_EXCEPTION_TRACING
}


void FileManager::deleteFile(const std::filesystem::path& r_filePath )
{
    CIE_BEGIN_EXCEPTION_TRACING

    // Get file path
    auto path = this->filePath( r_filePath );

    // Remove file if it exists
    if ( detail::isFile(path) )
        std::filesystem::remove( path );

    CIE_END_EXCEPTION_TRACING
}


const std::vector<std::filesystem::path> FileManager::listFiles( )
{
    CIE_BEGIN_EXCEPTION_TRACING

    std::vector<std::filesystem::path> fileList;

    for ( const auto& entry : std::filesystem::directory_iterator(_path) )
        if ( detail::isFile( entry.path().string() ) )
            fileList.push_back( detail::fileName( entry.path() ) );
    
    return fileList;

    CIE_END_EXCEPTION_TRACING
}


std::filesystem::path FileManager::filePath( const std::filesystem::path& r_fileName )
{
    CIE_BEGIN_EXCEPTION_TRACING

    return _path / detail::fileName( r_fileName );

    CIE_END_EXCEPTION_TRACING
}


} // namespace cie::utils