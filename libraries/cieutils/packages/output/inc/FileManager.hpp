#ifndef CIEUTILS_FILEMANAGER_HPP
#define CIEUTILS_FILEMANAGER_HPP

// --- STL Includes ---
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <filesystem>

namespace cie::utils {


using File      = std::fstream;
using FilePtr   = std::shared_ptr<File>;


class FileManager
{
public:
    FileManager();
    FileManager( const std::filesystem::path& r_path );
    ~FileManager();

    bool createPath( const std::filesystem::path& r_path );

    File& newFile( const std::filesystem::path& r_filePath );
    File& newFile( const std::filesystem::path& r_filePath,
                   std::ios_base::openmode openMode );
    File& open( const std::filesystem::path& r_filePath );

    FilePtr filePtr( const File& file );

    void deleteFile(const std::filesystem::path& r_filePath );
    void closeAll();

    const std::vector<std::filesystem::path> listFiles();

private:
    std::filesystem::path filePath(const std::filesystem::path& r_fileName );

    std::vector<FilePtr>                      _files;
    std::filesystem::path                     _path;
    static std::vector<std::filesystem::path> _paths;
};


} // namespace cie::utils


#endif