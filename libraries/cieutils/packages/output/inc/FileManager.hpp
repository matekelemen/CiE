#ifndef CIEUTILS_FILEMANAGER_HPP
#define CIEUTILS_FILEMANAGER_HPP

// --- STD Includes ---
#include <fstream>
#include <vector>
#include <string>
#include <memory>

namespace cie {
namespace utils {

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

    void deleteFile( const std::string& fileName );
    void closeAll();

    const std::vector<std::string> listFiles();

private:
    std::string filePath( const std::string& fileName );

    std::vector<FilePtr>                _files;
    const std::string                   _path;
    static std::vector<std::string>     _paths;
};


}
}


#endif