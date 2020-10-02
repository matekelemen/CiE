// --- Internal Includes ---
#include "cieutils/packages/output/inc/FileManager.hpp"
#include "cieutils/packages/output/inc/fileinfo.hpp"
#include "cieutils/packages/macros/inc/exceptions.hpp"
#include "cmake_variables.hpp"

// --- STL Includes ---
#include <filesystem>
#include <algorithm>


namespace cie::utils {


std::vector<std::string> FileManager::_paths = {};


FileManager::FileManager( const std::string& path ) :
    _path( path )
{
    if (_path.size() == 0)
        _path = INSTALL_PATH + "/output";

    if ( !detail::isDirectory(_path) )
        std::filesystem::create_directory( _path );

    auto it = std::find( _paths.begin(), _paths.end(), _path );
    if (it == _paths.end() )
        _paths.push_back(_path);
    //else
    //    CIE_THROW( std::runtime_error, "A FileManager is already active in " + _path )
}


FileManager::FileManager() :
    FileManager( INSTALL_PATH + "/output" )
{ 
}


FileManager::~FileManager()
{
    closeAll();
    auto it = std::find( _paths.begin(), _paths.end(), _path );
    if (it != _paths.end() )
        _paths.erase(it);
}


File& FileManager::newFile( const std::string& fileName )
{
    // Get file path
    auto path = filePath(fileName);
    std::ios_base::openmode mode = std::ios::out;

    // Determine modifier bits
    if ( detail::isBinaryExtension( detail::fileExtension(path) ) )
        mode = mode | std::ios::binary;

    // Register file stream
    _files.push_back( std::make_shared<std::fstream>( path, mode ) );
    return *_files[ _files.size()-1 ];
}


File& FileManager::open( const std::string& fileName )
{
    // Get file path
    auto path = filePath(fileName);

    // Check whether file exists
    if ( !detail::isFile(path) )
        throw std::runtime_error( path + " not found!" );

    // Determine modifier bits
    std::ios_base::openmode mode = std::ios::in;

    if ( detail::isBinaryExtension( detail::fileExtension(fileName) ) )
        mode = mode | std::ios::binary;

    // Register file stream
    _files.push_back( std::make_shared<std::fstream>( path, mode ) );
    return *_files[ _files.size()-1 ];
}


FilePtr FileManager::filePtr( const File& file )
{
    int index = -1;
    for (auto it=_files.begin(); it!=_files.end(); ++it)
        if (it->get() == &file)
        {
            index = std::distance( _files.begin(), it );
            break;
        }
    return _files[index];
}


void FileManager::closeAll()
{
    for ( auto it=_files.begin(); it!=_files.end(); ++it )
    {
        (*it)->close();
    }
}


void FileManager::deleteFile( const std::string& fileName )
{
    // Get file path
    auto path = filePath(fileName);

    // Remove file if it exists
    if ( detail::isFile(path) )
        std::filesystem::remove( path );
}


const std::vector<std::string> FileManager::listFiles( )
{
    std::vector<std::string> fileList;

    for ( const auto& entry : std::filesystem::directory_iterator(_path) )
        if ( detail::isFile( entry.path().string() ) )
            fileList.push_back( detail::fileName(entry.path().string()) );
    
    return fileList;
}


std::string FileManager::filePath( const std::string& fileName )
{
    return _path + "/" + detail::fileName(fileName);
}


} // namespace cie::utils