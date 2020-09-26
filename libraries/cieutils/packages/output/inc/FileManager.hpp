#ifndef CIEUTILS_FILEMANAGER_HPP
#define CIEUTILS_FILEMANAGER_HPP

// --- STL Includes ---
#include <fstream>
#include <vector>
#include <string>
#include <memory>

namespace cie::utils {

using File      = std::fstream;
using FilePtr   = std::shared_ptr<File>;


class FileManager
{
public:
    FileManager();
    FileManager( const std::string& path );
    ~FileManager();

    File& newFile( const std::string& fileName );
    File& open( const std::string& fileName );

    FilePtr filePtr( const File& file );

    void deleteFile( const std::string& fileName );
    void closeAll();

    const std::vector<std::string> listFiles();

private:
    std::string filePath( const std::string& fileName );

    std::vector<FilePtr>                _files;
    std::string                         _path;
    static std::vector<std::string>     _paths;
};


} // namespace cie::utils


#endif