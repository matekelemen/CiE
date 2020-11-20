#ifndef CIE_FILEINFO_HPP
#define CIE_FILEINFO_HPP

// --- STL Includes ---
#include <string>
#include <vector>
#include <filesystem>


namespace cie::utils::detail {


const std::vector<std::filesystem::path> textExtensions = {   ".txt",
                                                    ".csv"   };
const std::vector<std::filesystem::path> binaryExtensions = { ".bin",
                                                    ".b"     };



std::filesystem::path fileExtension(const std::filesystem::path& r_filePath );
std::filesystem::path fileName(const std::filesystem::path& r_filePath );
std::filesystem::path fileDirectory(const std::filesystem::path& r_filePath );

bool isTextExtension(const std::filesystem::path& r_extension);
bool isBinaryExtension(const std::filesystem::path& r_extension);

bool isFile(const std::filesystem::path& r_filePath );
bool isTextFile(const std::filesystem::path& r_filePath );
bool isBinaryFile(const std::filesystem::path& r_filePath );
bool isDirectory( const std::filesystem::path& r_filePath );


} // namespace cie::utils::detail

#endif